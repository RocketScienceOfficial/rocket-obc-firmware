#pragma once

#include <obc/api.h>

/**
 * @brief PID regulator data
 */
typedef struct PIDRegulatorData
{
    FLOAT kp;        /** KP Factor */
    FLOAT ki;        /** KI Factor */
    FLOAT kd;        /** KD Factor */
    FLOAT dt;        /** Time step */
    FLOAT lastError; /** Last error */
    FLOAT integral;  /** Integral */
} PIDRegulatorData;

/**
 * @brief Initialize PID regulator
 *
 * @param pid PID regulator data
 * @param kp Proportional gain
 * @param ki Integral gain
 * @param kd Derivative gain
 * @param dt Time step
 */
VOID pidRegulatorInit(PIDRegulatorData *pid, FLOAT kp, FLOAT ki, FLOAT kd, FLOAT dt);

/**
 * @brief Update PID regulator
 *
 * @param pid PID regulator data
 * @param expected Expected value (Setpoint)
 * @param actual Actual value
 * @return Control signal
 */
FLOAT pidRegulatorUpdate(PIDRegulatorData *pid, FLOAT expected, FLOAT actual);