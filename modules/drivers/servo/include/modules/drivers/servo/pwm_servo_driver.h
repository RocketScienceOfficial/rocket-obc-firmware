#ifndef _PWM_SERVO_DRIVER_H
#define _PWM_SERVO_DRIVER_H

#include "modules/drivers/hal/gpio_driver.h"
#include "modules/drivers/hal/pwm_driver.h"
#include <stdbool.h>

/**
 * @brief Initialize servo
 *
 * @param config PWM configuration
 * @param pin GPIO pin to use for servo
 */
void pwm_servo_init(hal_pwm_config_t *config, hal_pin_number_t pin);

/**
 * @brief Check if servo is connected
 *
 * @param config PWM configuration
 * @param result Result of check
 */
void pwm_servo_check(hal_pwm_config_t *config, bool *result);

/**
 * @brief Rotate servo to angle
 *
 * @param config PWM configuration
 * @param angle Angle to rotate to (degrees)
 */
void pwm_servo_rotate_angle(hal_pwm_config_t *config, float destAngleDegrees);

#endif