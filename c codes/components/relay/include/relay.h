#ifndef RELAY_H
#define RELAY_H

#include "driver/gpio.h"

// Define the relay control pin
#define RELAY_PIN 14

// Function prototypes
void relay_init(void);
void relay_turn_on(void);
void relay_turn_off(void);

#endif // RELAY_H
