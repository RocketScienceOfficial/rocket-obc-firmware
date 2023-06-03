#pragma once

#include <obc/api.h>
#include "drivers/gpio/gpio_driver.h"

/**
 * @brief Initialize servo
 *
 * @param pin GPIO pin to use for servo
 * @return Result code
 */
FUNCRESULT pwmServoInit(PinNumber pin);

/**
 * @brief Check if servo is connected
 *
 * @param pin GPIO pin to use for servo
 * @param result Result of check
 * @return Result code
 */
FUNCRESULT pwmServoCheck(PinNumber pin, BOOL *result);

/**
 * @brief Rotate servo to angle
 *
 * @param pin GPIO pin to use for servo
 * @param angle Angle to rotate to (degrees)
 * @return Result code
 */
FUNCRESULT pwmServoRotateAngle(PinNumber pin, FLOAT destAngleDegrees);