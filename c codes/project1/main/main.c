#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include "driver/gpio.h"
#include "driver/i2c.h"
#include "esp_log.h"
#include "nvs_flash.h"
#include "esp_netif.h"
#include "esp_http_client.h"
#include "esp_wifi.h"
#include "esp_event.h"

#include "ut.h"     // Food-level sensor
#include "svm.h"    // Servo control
#include "ut2.h"    // Water-level sensor
#include "relay.h"  // Pump control
#include "lcd_i2c.h"

// External Firebase certificate binary
extern const uint8_t firebase_cert_pem_start[] asm("_binary_firebase_cert_pem_start");
extern const uint8_t firebase_cert_pem_end[]   asm("_binary_firebase_cert_pem_end");

// WiFi Credentials
#define WIFI_SSID "Dhananjaya"
#define WIFI_PASS "tona6312"

// Firebase Endpoints
#define FIREBASE_URL      "https://smartfoodfeeder-default-rtdb.asia-southeast1.firebasedatabase.app/SmartFeeder.json?auth=KQotWNfDGqsVPda8vSfgshNderoDCwj0f9f5dVqQ"
#define FIREBASE_GET_PATH "https://smartfoodfeeder-default-rtdb.asia-southeast1.firebasedatabase.app/SmartFeeder/foodcount.json?auth=KQotWNfDGqsVPda8vSfgshNderoDCwj0f9f5dVqQ"

// LCD I2C configuration
#define I2C_MASTER_NUM     I2C_NUM_0
#define I2C_MASTER_SDA_IO  21
#define I2C_MASTER_SCL_IO  22
#define I2C_MASTER_FREQ_HZ 100000

// Food container and thresholds
#define FOOD_THRESHOLD       20
#define WATER_ON_HEIGHT      26
#define WATER_OFF_HEIGHT     20
#define CONTAINER_FULL_HEIGHT 16.0f

// Globals
static const char *TAG = "smart_feeder";
static EventGroupHandle_t wifi_event_group;
const int CONNECTED_BIT = BIT0;
volatile bool foodcount = false;

////////////////////////////////////////////////////////////////////////////////
// Wi‑Fi initialization and event handler
////////////////////////////////////////////////////////////////////////////////
static void wifi_event_handler(void* arg, esp_event_base_t event_base,
                               int32_t event_id, void* event_data)
{
    if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_START) {
        esp_wifi_connect();
    } else if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_DISCONNECTED) {
        esp_wifi_connect();
        xEventGroupClearBits(wifi_event_group, CONNECTED_BIT);
    } else if (event_base == IP_EVENT && event_id == IP_EVENT_STA_GOT_IP) {
        xEventGroupSetBits(wifi_event_group, CONNECTED_BIT);
    }
}

static void wifi_init_sta(void)
{
    wifi_event_group = xEventGroupCreate();
    ESP_ERROR_CHECK(esp_netif_init());
    ESP_ERROR_CHECK(esp_event_loop_create_default());
    esp_netif_create_default_wifi_sta();

    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&cfg));

    ESP_ERROR_CHECK(esp_event_handler_register(WIFI_EVENT, ESP_EVENT_ANY_ID,
                                               &wifi_event_handler, NULL));
    ESP_ERROR_CHECK(esp_event_handler_register(IP_EVENT, IP_EVENT_STA_GOT_IP,
                                               &wifi_event_handler, NULL));

    wifi_config_t wifi_config = {
        .sta = { .threshold.authmode = WIFI_AUTH_WPA2_PSK },
    };
    strlcpy((char *)wifi_config.sta.ssid,     WIFI_SSID, sizeof(wifi_config.sta.ssid));
    strlcpy((char *)wifi_config.sta.password, WIFI_PASS, sizeof(wifi_config.sta.password));

    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));
    ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_STA, &wifi_config));
    ESP_ERROR_CHECK(esp_wifi_start());
}

////////////////////////////////////////////////////////////////////////////////
// HTTP event handler (silent)
////////////////////////////////////////////////////////////////////////////////
static esp_err_t http_event_handler(esp_http_client_event_t *evt)
{
    return ESP_OK;
}

////////////////////////////////////////////////////////////////////////////////
// Helper: fetch a boolean from Firebase at `url` into `*out_value`.
// Returns true on success, false on error.
////////////////////////////////////////////////////////////////////////////////
bool get_firebase_bool(const char *url, bool *out_value)
{
    ESP_LOGI(TAG, "Fetching bool from: %s", url);

    esp_http_client_config_t config = {
        .url = url,
        .event_handler = http_event_handler,
        .cert_pem = (const char *)firebase_cert_pem_start,
        .skip_cert_common_name_check = true,
    };

    esp_http_client_handle_t client = esp_http_client_init(&config);
    if (!client) {
        ESP_LOGE(TAG, "Failed to init HTTP client");
        return false;
    }

    esp_http_client_set_method(client, HTTP_METHOD_GET);
    esp_err_t err = esp_http_client_open(client, 0);
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "HTTP open failed: %s", esp_err_to_name(err));
        esp_http_client_cleanup(client);
        return false;
    }

    int content_length = esp_http_client_fetch_headers(client);
    if (content_length <= 0) {
        ESP_LOGE(TAG, "Invalid content length");
        esp_http_client_cleanup(client);
        return false;
    }

    char buffer[64] = { 0 };
    int read_len = esp_http_client_read(client, buffer, sizeof(buffer) - 1);
    if (read_len < 0) {
        ESP_LOGE(TAG, "Read failed");
        esp_http_client_cleanup(client);
        return false;
    }
    buffer[read_len] = '\0';
    ESP_LOGI(TAG, "Firebase response: %s", buffer);

    if (strcmp(buffer, "true") == 0) {
        *out_value = true;
    } else if (strcmp(buffer, "false") == 0) {
        *out_value = false;
    } else {
        ESP_LOGE(TAG, "Unexpected response");
        esp_http_client_cleanup(client);
        return false;
    }

    esp_http_client_cleanup(client);
    return true;
}

////////////////////////////////////////////////////////////////////////////////
// Task: periodically GET `foodcount` from Firebase
////////////////////////////////////////////////////////////////////////////////
void get_from_firebase_task(void *pvParameters)
{
    while (1) {
        bool new_val = false;
        if (get_firebase_bool(FIREBASE_GET_PATH, &new_val)) {
            foodcount = new_val;
            ESP_LOGI(TAG, "Updated foodcount = %s", foodcount ? "true" : "false");
        } else {
            ESP_LOGE(TAG, "Failed to update foodcount");
        }
        //vTaskDelay(pdMS_TO_TICKS(5000));
    }
}

////////////////////////////////////////////////////////////////////////////////
// Task: periodically PUT both `foodcount` and `watercount` to Firebase
////////////////////////////////////////////////////////////////////////////////
void http_post_task(void *pvParameters)
{
    xEventGroupWaitBits(wifi_event_group, CONNECTED_BIT, false, true, portMAX_DELAY);

    esp_http_client_config_t config = {
        .url = FIREBASE_URL,
        .method = HTTP_METHOD_PUT,
        .event_handler = http_event_handler,
        .cert_pem = (const char *)firebase_cert_pem_start,
        .transport_type = HTTP_TRANSPORT_OVER_SSL,
    };

    esp_http_client_handle_t client = esp_http_client_init(&config);
    while (1) {
        char payload[128];
        snprintf(payload, sizeof(payload),
                 "{\"foodcount\": %s, \"watercount\": false}",
                 foodcount ? "true" : "false");

        esp_http_client_set_post_field(client, payload, strlen(payload));
        esp_http_client_set_header(client, "Content-Type", "application/json");
        esp_err_t err = esp_http_client_perform(client);
        if (err == ESP_OK) {
            ESP_LOGI(TAG, "Firebase PUT success");
        } else {
            ESP_LOGE(TAG, "Firebase PUT failed: %s", esp_err_to_name(err));
        }
        vTaskDelay(pdMS_TO_TICKS(5000));
    }
    // never reached:
    // esp_http_client_cleanup(client);
    // vTaskDelete(NULL);
}

////////////////////////////////////////////////////////////////////////////////
// I2C + LCD initialization
////////////////////////////////////////////////////////////////////////////////
static void i2c_master_init(void)
{
    i2c_config_t conf = {
        .mode = I2C_MODE_MASTER,
        .sda_io_num = I2C_MASTER_SDA_IO,
        .scl_io_num = I2C_MASTER_SCL_IO,
        .sda_pullup_en = GPIO_PULLUP_ENABLE,
        .scl_pullup_en = GPIO_PULLUP_ENABLE,
        .master.clk_speed = I2C_MASTER_FREQ_HZ,
    };
    i2c_param_config(I2C_MASTER_NUM, &conf);
    i2c_driver_install(I2C_MASTER_NUM, conf.mode, 0, 0, 0);
}

void display_food_status(float distance)
{
    lcd_clear();
    if (distance > FOOD_THRESHOLD) {
        lcd_print_centered(" FOOD OVER", 0);
        lcd_set_cursor(0, 1);
        lcd_print("Please refill!");
    } else {
        float height = CONTAINER_FULL_HEIGHT - distance;
        if (height < 0) height = 0;
        int pct = (int)((height / CONTAINER_FULL_HEIGHT) * 100);
        char line0[17], line1[17];
        snprintf(line0, sizeof(line0), " Food Level: %d%%", pct);
        snprintf(line1, sizeof(line1), "Height: %.1f cm", height);
        lcd_print_centered(line0, 0);
        lcd_print_centered(line1, 1);
    }
}

////////////////////////////////////////////////////////////////////////////////
// Sensor & actuator tasks
////////////////////////////////////////////////////////////////////////////////
void food_level_task(void *pvParameters)
{
    ultrasonic_init();
    while (1) {
        float d = ultrasonic_measure_distance();
        ESP_LOGI(TAG, "[Food] %.2f cm", d);
        display_food_status(d);
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

void servo_task(void *pvParameters)
{
    servo_init(GPIO_NUM_26);
    
    // Initialize servo to false position (130°)
    bool current_foodcount = false;
    servo_set_angle(0);
    ESP_LOGI(TAG, "Initial position: 130°");
    
    while (1) {
        // Only act when foodcount changes
        if (foodcount != current_foodcount) {
            current_foodcount = foodcount;
            
            if (current_foodcount) {
                servo_set_angle(130);  // Move to 0° for true
                ESP_LOGI(TAG, "App triggered: Moving to 0°");
            } else {
                servo_set_angle(0); // Move to 130° for false
                ESP_LOGI(TAG, "App triggered: Moving to 130°");
            }
        }
        vTaskDelay(pdMS_TO_TICKS(50)); // Short delay for responsiveness
    }
}

void water_level_task(void *pvParameters)
{
    ultrasonic2_init();
    relay_init();
    bool pump_on = false;
    while (1) {
        float d = ultrasonic2_measure_distance();
        ESP_LOGI(TAG, "[Water] %.2f cm", d);
        if (d >= 2 && d <= 400) {
            if (!pump_on && d < WATER_ON_HEIGHT) {
                relay_turn_on();
                pump_on = true;
                ESP_LOGI(TAG, "PUMP ON");
            } else if (pump_on && d > WATER_OFF_HEIGHT) {
                relay_turn_off();
                pump_on = false;
                ESP_LOGI(TAG, "PUMP OFF");
            }
        }
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

////////////////////////////////////////////////////////////////////////////////
// app_main
////////////////////////////////////////////////////////////////////////////////
void app_main(void)
{
    // LCD init
    i2c_master_init();
    lcd_init(I2C_MASTER_NUM);
    lcd_clear();
    lcd_print_centered("System Starting", 0);
    vTaskDelay(pdMS_TO_TICKS(2000));

    // Wi‑Fi + NVS
    ESP_ERROR_CHECK(nvs_flash_init());
    wifi_init_sta();
    xEventGroupWaitBits(wifi_event_group, CONNECTED_BIT, false, true, portMAX_DELAY);

    // Create tasks
    xTaskCreate(http_post_task,         "http_post", 8192, NULL, 5, NULL);
    xTaskCreate(get_from_firebase_task, "http_get",  8192, NULL, 5, NULL);
    xTaskCreate(food_level_task,        "food",     4096, NULL, 5, NULL);
    xTaskCreate(servo_task,             "servo",    2048, NULL, 5, NULL);
    xTaskCreate(water_level_task,       "water",    4096, NULL, 5, NULL);
}
