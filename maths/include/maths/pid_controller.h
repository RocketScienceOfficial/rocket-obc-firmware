#pragma once

#include <obc/api.h>

/**
 * @brief PID controller data
 */
typedef struct PIDControllerData
{
    FLOAT kp;        /** KP Factor */
    FLOAT ki;        /** KI Factor */
    FLOAT kd;        /** KD Factor */
    FLOAT dt;        /** Time step */
    FLOAT lastError; /** Last error */
    FLOAT integral;  /** Integral */
} PIDControllerData;

/**
 * @brief Initialize PID controller
 *
 * @param pid PID controller data
 * @param kp Proportional gain
 * @param ki Integral gain
 * @param kd Derivative gain
 * @param dt Time step
 */
VOID pidControllerInit(PIDControllerData *pid, FLOAT kp, FLOAT ki, FLOAT kd, FLOAT dt);

/**
 * @brief Update PID controller
 *
 * @param pid PID controller data
 * @param expected Expected value (Setpoint)
 * @param actual Actual value
 * @return Control signal
 */
FLOAT pidControllerUpdate(PIDControllerData *pid, FLOAT expected, FLOAT actual);