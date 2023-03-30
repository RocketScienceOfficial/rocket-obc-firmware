#pragma once

/**
 * REF: https://ahrs.readthedocs.io/en/latest/filters/madgwick.html
 */

#include <obc/api.h>
#include "maths/quaternion.h"
#include "maths/vector.h"

#define MADGWICK_BETA_CONSTANT 0.8660254f

/**
 * @brief Madgwick filter data for IMU
 */
typedef struct MadgiwckFilterDataIMU
{
    quat q;
    FLOAT beta;
    FLOAT samplePeriod;
    quat gyroQuat;
    quat gyroDerivative;
    vec3 func_g;
    vec3 jt[4];
    quat gradQuat;
} MadgiwckFilterDataIMU;

/**
 * @brief Madgwick filter data for MARG
 */
typedef struct MadgiwckFilterDataMARG
{
    quat q;
    FLOAT beta;
    FLOAT samplePeriod;
    quat gyroQuat;
    quat gyroDerivative;
    quat magQuat;
    quat qConj;
    quat h;
    quat b;
    vec3 func_g;
    vec3 func_b;
} MadgiwckFilterDataMARG;

/**
 * @brief Initialize Madgwick filter for IMU
 *
 * @param data Madgwick filter data for IMU
 * @param omegaError Gyroscope error in degrees per second
 * @param samplePeriod Sample period
 */
VOID madgwickInitIMU(MadgiwckFilterDataIMU *data, FLOAT omegaError, FLOAT samplePeriod);

/**
 * @brief Initialize Madgwick filter for MARG
 *
 * @param data Madgwick filter data for MARG
 * @param omegaError Gyroscope error in degrees per second
 * @param samplePeriod Sample period
 */
VOID madgwickInitMARG(MadgiwckFilterDataMARG *data, FLOAT omegaError, FLOAT samplePeriod);

/**
 * @brief Update Madgwick filter without using magnetometer
 *
 * @param data Madgwick filter data for IMU
 * @param gyroVec Gyrosopce vector in radians per second
 * @param accVec Accelerometer vector in meters per second squared
 */
VOID madgwickUpdateIMU(MadgiwckFilterDataIMU *data, vec3 gyroVec, vec3 accVec);

/**
 * @brief Update Madgwick filter using magnetometer
 *
 * @param data Madgwick filter data for MARG
 * @param gyroVec Gyrosopce vector in radians per second
 * @param accVec Accelerometer vector in meters per second squared
 * @param magVec Magnetometer vector in Gauss
 */
VOID madgwickUpdateMARG(MadgiwckFilterDataMARG *data, vec3 gyroVec, vec3 accVec, vec3 magVec);