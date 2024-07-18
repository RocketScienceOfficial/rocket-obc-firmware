#ifndef _PWM_SERVO_DRIVER_H
#define _PWM_SERVO_DRIVER_H

#include "hal/gpio_driver.h"
#include "hal/pwm_driver.h"
#include <stdbool.h>

/**
 * @brief Initialize servo
 *
 * @param config PWM configuration
 * @param pin GPIO pin to use for servo
 */
void pwm_servo_init(hal_pwm_config_t *config, hal_pin_number_t pin);

/**
 * @brief Rotate servo to angle
 *
 * @param config PWM configuration
 * @param angle Angle to rotate to (degrees)
 */
void pwm_servo_rotate_angle(const hal_pwm_config_t *config, float destAngleDegrees);

#endif