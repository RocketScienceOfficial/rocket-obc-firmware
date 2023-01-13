#pragma once

/**
 * INFO: https://ahrs.readthedocs.io/en/latest/filters/madgwick.html
 */

#include <obc/api.h>
#include "maths/quaternion.h"
#include "maths/vector.h"

/**
 * @brief Madgwick filter data.
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
 * @param data Madgwick filter data.
 * @param omegaError Gyroscope error.
 * @param samplePeriod Sample period.
 * @param useMagnetometer If set to TRUE, you will have to call -Mag version.
 */
VOID madgwickInit(MadgiwckFilterData *data, FLOAT omegaError, FLOAT samplePeriod, BOOL useMagnetometer);

/**
 * @brief Update Madgwick filter without using magnetometer.
 *
 * @param data Madgwick filter data.
 * @param gyroVec Gyrosopce vector.
 * @param accVec Accelerometer vector.
 */
VOID madgwickUpdate(MadgiwckFilterData *data, vec3 gyroVec, vec3 accVec);

/**
 * @brief Update Madgwick filter.
 *
 * @param data Madgwick filter data.
 * @param gyroVec Gyrosopce vector.
 * @param accVec Accelerometer vector.
 * @param magVec Magnetometer vector.
 */
VOID madgwickUpdateMag(MadgiwckFilterData *data, vec3 gyroVec, vec3 accVec, vec3 magVec);