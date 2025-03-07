#include <driver/ledc.h>

// Servo configuration
#define SERVO_PIN 26       // GPIO pin for the servo signal
#define SERVO_MIN_PULSE 500 // Minimum pulse width in microseconds (0 degrees)
#define SERVO_MAX_PULSE 2500 // Maximum pulse width in microseconds (180 degrees)
#define SERVO_MIN_DEGREE 0   // Minimum angle in degrees
#define SERVO_MAX_DEGREE 180 // Maximum angle in degrees

// LEDC configuration
#define LEDC_TIMER LEDC_TIMER_0
#define LEDC_MODE LEDC_LOW_SPEED_MODE
#define LEDC_CHANNEL LEDC_CHANNEL_0
#define LEDC_DUTY_RES LEDC_TIMER_13_BIT // 13-bit resolution (8192 steps)
#define LEDC_FREQUENCY 50 // 50 Hz frequency for servo control

// Function to set servo angle
void setServoAngle(int angle) {
    // Constrain the angle to the valid range
    if (angle < SERVO_MIN_DEGREE) angle = SERVO_MIN_DEGREE;
    if (angle > SERVO_MAX_DEGREE) angle = SERVO_MAX_DEGREE;

    // Map the angle to the corresponding pulse width
    uint32_t pulseWidth = SERVO_MIN_PULSE + (angle - SERVO_MIN_DEGREE) * (SERVO_MAX_PULSE - SERVO_MIN_PULSE) / (SERVO_MAX_DEGREE - SERVO_MIN_DEGREE);

    // Convert pulse width to duty cycle
    uint32_t duty = (pulseWidth * (1 << LEDC_DUTY_RES)) / (1000000 / LEDC_FREQUENCY);

    // Set the duty cycle
    ledc_set_duty(LEDC_MODE, LEDC_CHANNEL, duty);
    ledc_update_duty(LEDC_MODE, LEDC_CHANNEL);
}

void app_main() {
    // Configure LEDC timer
    ledc_timer_config_t timer_conf = {
        .speed_mode = LEDC_MODE,
        .duty_resolution = LEDC_DUTY_RES,
        .timer_num = LEDC_TIMER,
        .freq_hz = LEDC_FREQUENCY,
        .clk_cfg = LEDC_AUTO_CLK
    };
    ledc_timer_config(&timer_conf);

    // Configure LEDC channel
    ledc_channel_config_t channel_conf = {
        .gpio_num = SERVO_PIN,
        .speed_mode = LEDC_MODE,
        .channel = LEDC_CHANNEL,
        .intr_type = LEDC_INTR_DISABLE,
        .timer_sel = LEDC_TIMER,
        .duty = 0,
        .hpoint = 0
    };
    ledc_channel_config(&channel_conf);

    // Main loop
    while (1) {
        setServoAngle(0); // Move to 0 degrees
        vTaskDelay(1000 / portTICK_PERIOD_MS); // Delay for 1 second

        setServoAngle(90); // Move to 90 degrees
        vTaskDelay(1000 / portTICK_PERIOD_MS); // Delay for 1 second
    }
}