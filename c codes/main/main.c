#include <stdio.h>
#include <stdbool.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "ut.h"     // Food-level ultrasonic sensor interface
#include "svm.h"    // Servo motor control interface
#include "ut2.h"    // Water-level ultrasonic sensor interface
#include "relay.h"  // Relay control interface

#define FOOD_DISTANCE_THRESHOLD 20  // Food feeder distance threshold in cm
#define HEIGHT_ON  26               // Pump turns ON when water level exceeds 26 cm
#define HEIGHT_OFF 20               // Pump turns OFF when water level is below 20 cm

// Task for monitoring food level using the ultrasonic sensor (ut)
void food_level_task(void *pvParameters) {
    ultrasonic_init();  // Initialize food-level ultrasonic sensor
    while (1) {
        float distance = ultrasonic_measure_distance();
        printf("Food Level Distance: %.2f cm\n", distance);
        if (distance > FOOD_DISTANCE_THRESHOLD) {
            printf("Food is over!\n");
        }
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

// Task for controlling the servo motor (svm)
void servo_task(void *pvParameters) {
    servo_init(26);  // Initialize servo motor on GPIO 26
    while (1) {
        servo_set_angle(0);   // Open feeder (0 degrees)
        vTaskDelay(pdMS_TO_TICKS(1000));
        servo_set_angle(130); // Close feeder (130 degrees, adjust as needed)
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

// Task for monitoring water level and controlling the pump via relay (ut2 + relay)
void water_level_task(void *pvParameters) {
    ultrasonic2_init();  // Initialize water-level ultrasonic sensor (ut2)
    relay_init();        // Initialize relay for pump control
    while (1) {
        float distance = ultrasonic2_measure_distance();
        printf("Water Level Distance: %.2f cm\n", distance);
        if (distance < 2 || distance > 400) {
            printf("Invalid water level reading. Skipping...\n");
        } else if (distance > HEIGHT_ON) {
            relay_turn_on();
            printf("Pump ON\n");
        } else if (distance < HEIGHT_OFF) {
            relay_turn_off();
            printf("Pump OFF\n");
        }
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

void app_main(void) {
    xTaskCreate(food_level_task, "food_level_task", 2048, NULL, 5, NULL);
    xTaskCreate(servo_task, "servo_task", 2048, NULL, 5, NULL);
    xTaskCreate(water_level_task, "water_level_task", 2048, NULL, 5, NULL);
}
