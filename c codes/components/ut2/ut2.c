#include "ut2.h"
#include "driver/gpio.h"
#include "esp_timer.h"
#include "esp_rom_sys.h"    // For esp_rom_delay_us
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

void ultrasonic2_init(void) {
    // Initialize the ultrasonic sensor
    esp_rom_gpio_pad_select_gpio(UT2_TRIG_PIN);
    esp_rom_gpio_pad_select_gpio(UT2_ECHO_PIN);
    gpio_set_direction(UT2_TRIG_PIN, GPIO_MODE_OUTPUT);
    gpio_set_direction(UT2_ECHO_PIN, GPIO_MODE_INPUT);
    gpio_set_level(UT2_TRIG_PIN, 0);  // Ensure trigger is low initially
    vTaskDelay(pdMS_TO_TICKS(50));    // Allow sensor to stabilize
}

float ultrasonic2_measure_distance(void) {
    int64_t start_time, end_time;
    float distance;

    // Trigger pulse
    gpio_set_level(UT2_TRIG_PIN, 0);
    vTaskDelay(pdMS_TO_TICKS(2));
    gpio_set_level(UT2_TRIG_PIN, 1);
    esp_rom_delay_us(10);
    gpio_set_level(UT2_TRIG_PIN, 0);

    // Wait for echo to go HIGH
    int64_t timeout_start = esp_timer_get_time();
    while (gpio_get_level(UT2_ECHO_PIN) == 0) {
        if (esp_timer_get_time() - timeout_start > 30000) return -1;  // Timeout handling
    }
    start_time = esp_timer_get_time();

    // Wait for echo to go LOW
    timeout_start = start_time;
    while (gpio_get_level(UT2_ECHO_PIN) == 1) {
        if (esp_timer_get_time() - timeout_start > 30000) return -1;  // Timeout handling
    }
    end_time = esp_timer_get_time();

    // Calculate distance in cm
    distance = (end_time - start_time) * 0.017;

    // Return -1 if out of range (valid range: 2cm - 400cm)
    if (distance < 2 || distance > 400) return -1;

    return distance;
}
