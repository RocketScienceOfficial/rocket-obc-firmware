#include "lib/drivers/servo/pwm_servo_driver.h"
#include "hal/pwm_driver.h"

static const unsigned int SERVO_FREQ_HZ = 50;
static const float SERVO_DUTY_CYCLE_MS_0 = 0.5f;
static const float SERVO_DUTY_CYCLE_MS_90 = 1.5f;
static const float SERVO_DUTY_CYCLE_MS_180 = 2.5f;
static const float SERVO_MAX_ANGLE_DEG = 180.0f;

void pwm_servo_init(hal_pwm_config_t *config, hal_pin_number_t pin)
{
    hal_pwm_init_pin(config, pin, SERVO_FREQ_HZ);
}

void pwm_servo_rotate_angle(hal_pwm_config_t *config, float destAngleDegrees)
{
    float duty = ((SERVO_DUTY_CYCLE_MS_180 - SERVO_DUTY_CYCLE_MS_0) / SERVO_MAX_ANGLE_DEG * destAngleDegrees + SERVO_DUTY_CYCLE_MS_0) * SERVO_FREQ_HZ / 1000.0f;

    hal_pwm_set_duty(config, duty);
}