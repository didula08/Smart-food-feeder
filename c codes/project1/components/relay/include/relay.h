#ifndef RELAY_H
#define RELAY_H

#include "driver/gpio.h"

#define RELAY_PIN GPIO_NUM_14  // GPIO14 for relay

void relay_init(void);
void relay_turn_on(void);
void relay_turn_off(void);

#endif /* RELAY_H */