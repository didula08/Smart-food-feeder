#include <stdio.h>
#include "driver/gpio.h"      // GPIO driver for ESP32
#include "driver/timer.h"     // Timer driver for ESP32
#include "esp_log.h"          // Logging library for ESP32
#include "freertos/FreeRTOS.h" // FreeRTOS for task management
#include "freertos/task.h"    // FreeRTOS task functions

// Pin Definitions
#define TRIG_PIN GPIO_NUM_13  // GPIO 13 for Trig pin of the ultrasonic sensor
#define ECHO_PIN GPIO_NUM_12  // GPIO 12 for Echo pin of the ultrasonic sensor
#define RELAY_PIN GPIO_NUM_14 // GPIO 14 for Relay control

// Height Thresholds (in cm)
#define HEIGHT_ON 26  // Pump turns ON when water level exceeds 26 cm
#define HEIGHT_OFF 20 // Pump turns OFF when water level is below 20 cm

// Function to measure distance using the ultrasonic sensor
float measureDistance() {
    uint32_t duration = 0;

    // Send a 10 µs pulse to the Trig pin
    gpio_set_level(TRIG_PIN, 0);  // Set Trig pin LOW
    ets_delay_us(2);              // Wait for 2 microseconds
    gpio_set_level(TRIG_PIN, 1);  // Set Trig pin HIGH
    ets_delay_us(10);             // Wait for 10 microseconds
    gpio_set_level(TRIG_PIN, 0);  // Set Trig pin LOW

    // Measure the pulse duration from the Echo pin
    while (gpio_get_level(ECHO_PIN) == 0); // Wait for the Echo pin to go HIGH
    uint32_t start = esp_timer_get_time();  // Record the start time
    while (gpio_get_level(ECHO_PIN) == 1);  // Wait for the Echo pin to go LOW
    uint32_t end = esp_timer_get_time();    // Record the end time

    duration = end - start; // Calculate the pulse duration in microseconds

    // Calculate distance in centimeters
    float distance = duration * 0.034 / 2; // Speed of sound = 0.034 cm/µs
    return distance;
}

void app_main() {
    // Configure GPIO pins
    gpio_config_t io_conf;

    // Configure Trig and Relay pins as outputs
    io_conf.intr_type = GPIO_INTR_DISABLE;  // Disable interrupts
    io_conf.mode = GPIO_MODE_OUTPUT;        // Set as output mode
    io_conf.pin_bit_mask = (1ULL << TRIG_PIN) | (1ULL << RELAY_PIN); // Select pins
    io_conf.pull_down_en = GPIO_PULLDOWN_DISABLE; // Disable pull-down
    io_conf.pull_up_en = GPIO_PULLUP_DISABLE;     // Disable pull-up
    gpio_config(&io_conf); // Apply the configuration

    // Configure Echo pin as input
    io_conf.mode = GPIO_MODE_INPUT;         // Set as input mode
    io_conf.pin_bit_mask = (1ULL << ECHO_PIN); // Select Echo pin
    gpio_config(&io_conf); // Apply the configuration

    // Initialize the Serial Monitor
    esp_log_level_set("*", ESP_LOG_INFO); // Set log level to INFO

    // Ensure the pump is OFF initially
    gpio_set_level(RELAY_PIN, 1); // HIGH means relay is OFF (for active LOW relay)

    while (1) {
        // Measure distance
        float distance = measureDistance();

        // Validate distance reading
        if (distance < 2 || distance > 400) {
            ESP_LOGI("Ultrasonic", "Invalid distance reading. Skipping...");
            vTaskDelay(1000 / portTICK_PERIOD_MS);
            continue; // Skip this iteration if the reading is invalid
        }

        // Print distance to Serial Monitor
        ESP_LOGI("Ultrasonic", "Distance: %.2f cm", distance);

        // Control the pump based on water level
        if (distance > HEIGHT_ON) {
            // If water level is above 26 cm, turn ON the pump
            gpio_set_level(RELAY_PIN, 0); // LOW means relay is ON (for active LOW relay)
            ESP_LOGI("Ultrasonic", "Pump ON");
        } else if (distance < HEIGHT_OFF) {
            // If water level is below 20 cm, turn OFF the pump
            gpio_set_level(RELAY_PIN, 1); // HIGH means relay is OFF (for active LOW relay)
            ESP_LOGI("Ultrasonic", "Pump OFF");
        }

        // Wait for 1 second before the next measurement
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}