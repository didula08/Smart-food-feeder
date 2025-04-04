#include "relay.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

void relay_init(void) {
    // Configure GPIO14 as an output
    gpio_reset_pin(RELAY_PIN);  // Reset the GPIO before use
    gpio_set_direction(RELAY_PIN, GPIO_MODE_OUTPUT);
    gpio_set_level(RELAY_PIN, 0); // Ensure relay is OFF initially
}

void relay_turn_on(void) {
    gpio_set_level(RELAY_PIN, 1); // Turn relay ON
}

void relay_turn_off(void) {
    gpio_set_level(RELAY_PIN, 0); // Turn relay OFF
}
