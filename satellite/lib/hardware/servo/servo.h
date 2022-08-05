#pragma once

#include "errors_types.h"

/**
 * @brief Data used to control servo
 */
typedef struct ServoData
{
    /**
     * @brief Servo pin
     */
    int pin;

    /**
     * @brief Servo rotation in degrees
     */
    float angle;
} ServoData;

/**
 * @brief Initialize servo
 *
 * @param pin GPIO pin to use for servo
 * @param data Data to setup
 * @return Result code
 */
FUNCRESULT servoInit(const int pin, ServoData *data);

/**
 * @brief Rotate servo to angle
 *
 * @param data Servo data
 * @param angle Angle to rotate to (degrees)
 * @return Result code
 */
FUNCRESULT servoRotateAngle(ServoData *data, float destAngleDegrees);