#pragma once

/**
 * INFO: https://www.kalmanfilter.net
 */

#include <obc/api.h>
#include "maths/vector.h"

/**
 * @brief Kalman filter output data
 */
typedef struct KalmanOutputData
{
    vec3 acc; /** Acceleration vector. */
    vec3 vel; /** Velocity vector. */
    vec3 pos; /** Position vector.*/
} KalmanOutputData;

/**
 * @brief Update Kalman filter.
 *
 * @param output Kalman filter output data.
 * @param acc Acceleration vector.
 */
VOID kalmanUpdate(KalmanOutputData *output, vec3 acc);