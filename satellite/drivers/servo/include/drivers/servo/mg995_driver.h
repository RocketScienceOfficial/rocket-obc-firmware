#pragma once

#include "tools/typedefs.h"
#include "drivers/gpio/gpio_driver.h"

/**
 * @brief Initialize servo
 *
 * @param pin GPIO pin to use for servo
 * @return Result code
 */
FUNCRESULT mg995Init(PinNumber pin);

/**
 * @brief Rotate servo to angle
 *
 * @param pin GPIO pin to use for servo
 * @param angle Angle to rotate to (degrees)
 * @return Result code
 */
FUNCRESULT mg995RotateAngle(PinNumber pin, FLOAT destAngleDegrees);