#include <stdio.h>
#include "driver/gpio.h"
#include "driver/timer.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

// Pin Definitions
#define TRIG_PIN GPIO_NUM_18  // GPIO 18 for Trig
#define ECHO_PIN GPIO_NUM_19  // GPIO 19 for Echo

// Distance Threshold (in cm)
#define DISTANCE_THRESHOLD 14 // Send message if distance exceeds 14 cm

// Function to measure distance using the ultrasonic sensor
float measureDistance() {
    uint32_t duration = 0;

    // Send a 10 µs pulse to the Trig pin
    gpio_set_level(TRIG_PIN, 0);
    ets_delay_us(2);
    gpio_set_level(TRIG_PIN, 1);
    ets_delay_us(10);
    gpio_set_level(TRIG_PIN, 0);

    // Measure the pulse duration from the Echo pin
    while (gpio_get_level(ECHO_PIN) == 0); // Wait for the echo pin to go HIGH
    uint32_t start = esp_timer_get_time();  // Record the start time
    while (gpio_get_level(ECHO_PIN) == 1); // Wait for the echo pin to go LOW
    uint32_t end = esp_timer_get_time();    // Record the end time

    duration = end - start; // Calculate the pulse duration in microseconds

    // Calculate distance in centimeters
    float distance = duration * 0.034 / 2;
    return distance;
}

void app_main() {
    // Configure GPIO pins
    gpio_config_t io_conf;
    io_conf.intr_type = GPIO_INTR_DISABLE;
    io_conf.mode = GPIO_MODE_OUTPUT;
    io_conf.pin_bit_mask = (1ULL << TRIG_PIN);
    io_conf.pull_down_en = GPIO_PULLDOWN_DISABLE;
    io_conf.pull_up_en = GPIO_PULLUP_DISABLE;
    gpio_config(&io_conf);

    io_conf.mode = GPIO_MODE_INPUT;
    io_conf.pin_bit_mask = (1ULL << ECHO_PIN);
    gpio_config(&io_conf);

    // Initialize the Serial Monitor
    esp_log_level_set("*", ESP_LOG_INFO);

    while (1) {
        // Measure distance
        float distance = measureDistance();

        // Print distance to Serial Monitor
        ESP_LOGI("Ultrasonic", "Distance: %.2f cm", distance);

        // Check if distance exceeds the threshold
        if (distance > DISTANCE_THRESHOLD) {
            ESP_LOGI("Ultrasonic", "Food is over"); // Send message if distance > 14 cm
        }

        // Wait for 1 second before the next measurement
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}