#ifndef SVM_H
#define SVM_H

#include <stdint.h>

void servo_init(uint8_t gpio_num);
void servo_set_angle(int angle);

#endif // SVM_H
