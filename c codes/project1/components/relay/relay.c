#include "relay.h"
#include "driver/gpio.h"

void relay_init(void) {
    gpio_reset_pin(RELAY_PIN);
    gpio_set_direction(RELAY_PIN, GPIO_MODE_OUTPUT);
    gpio_set_level(RELAY_PIN, 0);  // Default OFF
}

void relay_turn_on(void) {
    gpio_set_level(RELAY_PIN, 1);  // Active HIGH
}

void relay_turn_off(void) {
    gpio_set_level(RELAY_PIN, 0);
}