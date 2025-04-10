#ifndef UT2_H
#define UT2_H

#include "driver/gpio.h"
#ifndef RELAY_H
#define RELAY_H

#include "driver/gpio.h"

#define RELAY_PIN GPIO_NUM_14  // GPIO14 for relay

void relay_init(void);
void relay_turn_on(void);
void relay_turn_off(void);

#endif
#define UT2_TRIG_PIN GPIO_NUM_13  // Define trigger pin
#define UT2_ECHO_PIN GPIO_NUM_12  // Define echo pin

void ultrasonic2_init(void);
float ultrasonic2_measure_distance(void);

#endif // UT2_H
