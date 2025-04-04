#ifndef ULTRASONIC_H
#define ULTRASONIC_H

#include "driver/gpio.h"

// Define sensor pins
#define TRIG_PIN 18
#define ECHO_PIN 19

// Distance threshold
#define DISTANCE_THRESHOLD 14  

// Function prototypes
void ultrasonic_init();
float ultrasonic_measure_distance();

#endif // ULTRASONIC_H
