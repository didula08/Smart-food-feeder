#include "ut.h"
#include "driver/gpio.h"
#include "esp_timer.h"
#include "esp_rom_sys.h"    // For esp_rom_delay_us
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

void ultrasonic_init(void) {
    // Initialize the food-level ultrasonic sensor
    esp_rom_gpio_pad_select_gpio(TRIG_PIN);
    esp_rom_gpio_pad_select_gpio(ECHO_PIN);
    gpio_set_direction(TRIG_PIN, GPIO_MODE_OUTPUT);
    gpio_set_direction(ECHO_PIN, GPIO_MODE_INPUT);
}

float ultrasonic_measure_distance(void) {
    int64_t start_time, end_time;
    float distance;

    // Trigger pulse
    gpio_set_level(TRIG_PIN, 0);
    vTaskDelay(pdMS_TO_TICKS(2));
    gpio_set_level(TRIG_PIN, 1);
    esp_rom_delay_us(10);
    gpio_set_level(TRIG_PIN, 0);

    // Wait for echo high
    while(gpio_get_level(ECHO_PIN) == 0);
    start_time = esp_timer_get_time();

    // Wait for echo low
    while(gpio_get_level(ECHO_PIN) == 1);
    end_time = esp_timer_get_time();

    distance = (end_time - start_time) * 0.034 / 2;
    return distance;
}
