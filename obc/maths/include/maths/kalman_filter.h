#pragma once

/**
 * REF: https://www.kalmanfilter.net
 * REF: https://www.bzarg.com/p/how-a-kalman-filter-works-in-pictures/
 */

#include <obc/api.h>
#include "maths/vector.h"

typedef struct KalmanDataIMU
{
} KalmanDataIMU;

/**
 * @brief Kalman filter output data
 */
typedef struct KalmanOutputDataIMU
{
    vec3 acc; /** Acceleration vector */
    vec3 vel; /** Velocity vector */
    vec3 pos; /** Position vector */
} KalmanOutputDataIMU;

/**
 * @brief Initialize Kalman filter for IMU
 *
 * @param data Kalman filter data
 */
VOID kalmanInitIMU(KalmanDataIMU *data);

/**
 * @brief Update Kalman filter for IMU
 *
 * @param data Kalman filter data
 * @param output Kalman filter output data
 * @param acc Acceleration vector
 */
VOID kalmanUpdateIMU(KalmanDataIMU *data, KalmanOutputDataIMU *output, vec3 acc);