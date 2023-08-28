#pragma once

#include <obc/api.h>
#include "drivers/gpio/gpio_driver.h"
#include "drivers/gpio/pwm_driver.h"

/**
 * @brief Initialize servo
 *
 * @param config PWM configuration
 * @param pin GPIO pin to use for servo
 * @return Result code
 */
FUNCRESULT pwmServoInit(PWMConfig *config, PinNumber pin);

/**
 * @brief Check if servo is connected
 *
 * @param config PWM configuration
 * @param result Result of check
 * @return Result code
 */
FUNCRESULT pwmServoCheck(PWMConfig *config, BOOL *result);

/**
 * @brief Rotate servo to angle
 *
 * @param config PWM configuration
 * @param angle Angle to rotate to (degrees)
 * @return Result code
 */
FUNCRESULT pwmServoRotateAngle(PWMConfig *config, FLOAT destAngleDegrees);