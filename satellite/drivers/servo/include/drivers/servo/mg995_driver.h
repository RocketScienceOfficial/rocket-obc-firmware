#pragma once

#include "tools/typedefs.h"
#include "drivers/gpio/gpio_driver.h"

/**
 * @brief Data used to control servo
 */
typedef struct MG995Data
{
    /**
     * @brief Servo pin
     */
    PinNumber pin;

    /**
     * @brief Servo rotation in degrees
     */
    FLOAT angle;
} MG995Data;

/**
 * @brief Initialize servo
 *
 * @param pin GPIO pin to use for servo
 * @param data Data to setup
 * @return Result code
 */
FUNCRESULT mg995Init(PinNumber pin, MG995Data *data);

/**
 * @brief Rotate servo to angle
 *
 * @param data Servo data
 * @param angle Angle to rotate to (degrees)
 * @return Result code
 */
FUNCRESULT mg995RotateAngle(MG995Data *data, FLOAT destAngleDegrees);