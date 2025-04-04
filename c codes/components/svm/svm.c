#include "svm.h"
#include "driver/mcpwm.h"
#include "esp_log.h"

void servo_init(uint8_t gpio_num)
{
    // Initialize MCPWM GPIO pin for the servo (channel A)
    mcpwm_gpio_init(MCPWM_UNIT_0, MCPWM0A, gpio_num);

    // Configure MCPWM for servo control: 50 Hz frequency (20ms period)
    mcpwm_config_t pwm_config = {
        .frequency = 50,          // 50 Hz
        .cmpr_a = 7.5,            // Neutral position (1.5 ms pulse, ~7.5% duty)
        .cmpr_b = 0.0,            // Unused
        .counter_mode = MCPWM_UP_COUNTER,
        .duty_mode = MCPWM_DUTY_MODE_0,
    };
    mcpwm_init(MCPWM_UNIT_0, MCPWM_TIMER_0, &pwm_config);
}

void servo_set_angle(int angle)
{
    // Clamp angle between 0 and 180 degrees
    if (angle < 0) angle = 0;
    if (angle > 180) angle = 180;

    // Map 0° to 1ms pulse (5% duty) and 180° to 2ms pulse (10% duty)
    float duty_cycle = 5.0 + ((float)angle / 180.0) * 5.0;

    // Update duty cycle for channel A
    mcpwm_set_duty(MCPWM_UNIT_0, MCPWM_TIMER_0, MCPWM_OPR_A, duty_cycle);
    mcpwm_set_duty_type(MCPWM_UNIT_0, MCPWM_TIMER_0, MCPWM_OPR_A, MCPWM_DUTY_MODE_0);
}
