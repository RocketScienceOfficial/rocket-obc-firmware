#ifndef _MAHONY_FILTER_H
#define _MAHONY_FILTER_H

/**
 * REF: https://github.com/PaulStoffregen/MahonyAHRS/blob/master/src/MahonyAHRS.cpp
 * REF: https://nitinjsanket.github.io/tutorials/attitudeest/mahony.html
 */

#include "modules/maths/quaternion.h"
#include "modules/maths/vector.h"

/**
 * @brief Mahony filter data
 */
typedef struct mahony_filter_data
{
    quat_t q;           /** Quaternion */
    float Kp;           /** Proportional gain */
    float Ki;           /** Integral gain */
    float integralFBx;  /** Integral X error */
    float integralFBy;  /** Integral Y error */
    float integralFBz;  /** Integral Z error */
    float samplePeriod; /** Sample period in seconds */
} mahony_filter_data_t;

/**
 * @brief Initialize Mahony filter. Remember that the device should be aligned with the earth frame (or device frame if you want to use it)
 *
 * @param data Mahony filter data
 * @param kp Proportional gain (Optimal value: 0.5)
 * @param ki Integral gain (Optimal value: 0.0)
 * @param samplePeriod Sample period
 */
void mahony_init(mahony_filter_data_t *data, float kp, float ki, float samplePeriod);

/**
 * @brief Update Mahony filter without using magnetometer
 *
 * @param data Mahony filter data
 * @param gyroVec Gyrosopce vector in radians per second
 * @param accVec Accelerometer vector in meters per second squared
 */
void mahony_update_imu(mahony_filter_data_t *data, vec3_t gyroVec, vec3_t accVec);

/**
 * @brief Update Mahony filter using magnetometer
 *
 * @param data Mahony filter data
 * @param gyroVec Gyrosopce vector in radians per second
 * @param accVec Accelerometer vector in meters per second squared
 * @param magVec Magnetometer vector in Gauss
 */
void mahony_update_marg(mahony_filter_data_t *data, vec3_t gyroVec, vec3_t accVec, vec3_t magVec);

#endif