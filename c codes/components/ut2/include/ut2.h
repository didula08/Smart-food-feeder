#ifndef UT2_H
#define UT2_H

#include "driver/gpio.h"

#define UT2_TRIG_PIN GPIO_NUM_13  // Define trigger pin
#define UT2_ECHO_PIN GPIO_NUM_12  // Define echo pin

void ultrasonic2_init(void);
float ultrasonic2_measure_distance(void);

#endif // UT2_H
