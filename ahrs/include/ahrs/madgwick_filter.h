#pragma once

/**
 * REF: https://ahrs.readthedocs.io/en/latest/filters/madgwick.html
 * REF: https://github.com/adafruit/Adafruit_AHRS/blob/master/src/Adafruit_AHRS_Madgwick.cpp
 * REF: https://github.com/arduino-libraries/MadgwickAHRS
 */

#include <obc/api.h>
#include "maths/quaternion.h"
#include "maths/vector.h"

/**
 * @brief Madgwick filter data for IMU
 */
typedef struct MadgiwckFilterData
{
    quat q;             /** Current calculated quaternion as orientation */
    FLOAT beta;         /** Filter coefficent */
    FLOAT samplePeriod; /** Inverse of frequency */
} MadgiwckFilterData;

/**
 * @brief Initialize Madgwick filter. Remember that the device should be aligned with the earth frame (or device frame if you want to use it)
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