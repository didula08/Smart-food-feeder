#include "ut2.h"
#include "esp_timer.h"
#include "esp_rom_sys.h"
#include "freertos/FreeRTOS.h"   
#include "freertos/task.h"      

void ultrasonic2_init(void) {
    gpio_reset_pin(UT2_TRIG_PIN);
    gpio_reset_pin(UT2_ECHO_PIN);
    gpio_set_direction(UT2_TRIG_PIN, GPIO_MODE_OUTPUT);
    gpio_set_direction(UT2_ECHO_PIN, GPIO_MODE_INPUT);
    gpio_set_level(UT2_TRIG_PIN, 0);
    vTaskDelay(pdMS_TO_TICKS(50));  // Stabilize
}

float ultrasonic2_measure_distance(void) {
    // Send 10µs trigger pulse
    gpio_set_level(UT2_TRIG_PIN, 1);
    esp_rom_delay_us(10);
    gpio_set_level(UT2_TRIG_PIN, 0);

    // Wait for echo rise (timeout: 30ms)
    int64_t timeout = esp_timer_get_time();
    while (!gpio_get_level(UT2_ECHO_PIN)) {
        if (esp_timer_get_time() - timeout > 30000) return -1;
    }
    int64_t start = esp_timer_get_time();

    // Wait for echo fall (timeout: 30ms)
    timeout = start;
    while (gpio_get_level(UT2_ECHO_PIN)) {
        if (esp_timer_get_time() - timeout > 30000) return -1;
    }
    int64_t end = esp_timer_get_time();

    // Calculate distance (cm)
    float distance = (end - start) * 0.0343 / 2;
    return (distance < 2 || distance > 400) ? -1 : distance;
}