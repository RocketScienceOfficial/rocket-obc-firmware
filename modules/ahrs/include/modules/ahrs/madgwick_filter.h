#ifndef _MADGWICK_FILTER_H
#define _MADGWICK_FILTER_H

/**
 * REF: https://ahrs.readthedocs.io/en/latest/filters/madgwick.html
 * REF: https://github.com/adafruit/Adafruit_AHRS/blob/master/src/Adafruit_AHRS_Madgwick.cpp
 * REF: https://github.com/arduino-libraries/MadgwickAHRS
 */

#include "modules/maths/quaternion.h"
#include "modules/maths/vector.h"

/**
 * @brief Madgwick filter data for IMU
 */
typedef struct madgiwck_filter_data
{
    quat_t q;           /** Current calculated quat_ternion as orientation */
    float beta;         /** Filter coefficent */
    float samplePeriod; /** Inverse of frequency */
} madgiwck_filter_data_t;

/**
 * @brief Initialize Madgwick filter. Remember that the device should be aligned with the earth frame (or device frame if you want to use it)
 *
 * @param data Madgwick filter data
 * @param omegaError Gyroscope error in degrees per second
 * @param samplePeriod Sample period
 */
void madgwick_init(madgiwck_filter_data_t *data, float omegaError, float samplePeriod);

/**
 * @brief Update Madgwick filter without using magnetometer
 *
 * @param data Madgwick filter data
 * @param gyroVec Gyrosopce vector in radians per second
 * @param accVec Accelerometer vector in meters per second squared
 */
void madgwick_update_imu(madgiwck_filter_data_t *data, vec3_t gyroVec, vec3_t accVec);

/**
 * @brief Update Madgwick filter using magnetometer
 *
 * @param data Madgwick filter data
 * @param gyroVec Gyrosopce vector in radians per second
 * @param accVec Accelerometer vector in meters per second squared
 * @param magVec Magnetometer vector in Gauss
 */
void madgwick_update_marg(madgiwck_filter_data_t *data, vec3_t gyroVec, vec3_t accVec, vec3_t magVec);

#endif