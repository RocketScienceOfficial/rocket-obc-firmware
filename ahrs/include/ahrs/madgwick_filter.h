#pragma once

/**
 * REF: https://ahrs.readthedocs.io/en/latest/filters/madgwick.html
 * REF: https://github.com/adafruit/Adafruit_AHRS/blob/master/src/Adafruit_AHRS_Madgwick.cpp
 */

#include <obc/api.h>
#include "maths/quaternion.h"
#include "maths/vector.h"

#define MADGWICK_BETA_CONSTANT 0.8660254f

/**
 * @brief Madgwick filter data for IMU
 */
typedef struct MadgiwckFilterData
{
    quat q;
    FLOAT beta;
    FLOAT samplePeriod;
} MadgiwckFilterData;

/**
 * @brief Initialize Madgwick filter
 *
 * @param data Madgwick filter data
 * @param omegaError Gyroscope error in degrees per second
 * @param samplePeriod Sample period
 */
VOID madgwickInit(MadgiwckFilterData *data, FLOAT omegaError, FLOAT samplePeriod);

/**
 * @brief Update Madgwick filter without using magnetometer
 *
 * @param data Madgwick filter data
 * @param gyroVec Gyrosopce vector in radians per second
 * @param accVec Accelerometer vector in meters per second squared
 */
VOID madgwickUpdateIMU(MadgiwckFilterData *data, vec3 gyroVec, vec3 accVec);

/**
 * @brief Update Madgwick filter using magnetometer
 *
 * @param data Madgwick filter data
 * @param gyroVec Gyrosopce vector in radians per second
 * @param accVec Accelerometer vector in meters per second squared
 * @param magVec Magnetometer vector in Gauss
 */
VOID madgwickUpdateMARG(MadgiwckFilterData *data, vec3 gyroVec, vec3 accVec, vec3 magVec);