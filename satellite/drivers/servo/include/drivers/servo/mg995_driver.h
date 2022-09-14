#pragma once

#include "drivers/hwutils/errors_types.h"

/**
 * @brief Data used to control servo
 */
typedef struct MG995Data
{
    /**
     * @brief Servo pin
     */
    int pin;

    /**
     * @brief Servo rotation in degrees
     */
    float angle;
} MG995Data;

/**
 * @brief Initialize servo
 *
 * @param pin GPIO pin to use for servo
 * @param data Data to setup
 * @return Result code
 */
FUNCRESULT mg995Init(const int pin, MG995Data *data);

/**
 * @brief Rotate servo to angle
 *
 * @param data Servo data
 * @param angle Angle to rotate to (degrees)
 * @return Result code
 */
FUNCRESULT mg995RotateAngle(MG995Data *data, float destAngleDegrees);