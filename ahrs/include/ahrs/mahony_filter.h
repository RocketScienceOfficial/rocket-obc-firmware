#pragma once

/**
 * REF: https://github.com/PaulStoffregen/MahonyAHRS/blob/master/src/MahonyAHRS.cpp
 * REF: https://nitinjsanket.github.io/tutorials/attitudeest/mahony.html
 */

#include <obc/api.h>
#include "maths/vector.h"
#include "maths/quaternion.h"

/**
 * @brief Mahony filter data
 */
typedef struct MahonyFilterData
{
    quat q;
    FLOAT Kp;
    FLOAT Ki;
    FLOAT integralFBx;
    FLOAT integralFBy;
    FLOAT integralFBz;
    FLOAT samplePeriod;
} MahonyFilterData;

/**
 * @brief Initialize Mahony filter. Remember that the device should be aligned with the earth frame (or device frame if you want to use it)
 *
 * @param data Mahony filter data
 * @param kp Proportional gain (Optimal value: 0.5)
 * @param ki Integral gain (Optimal value: 0.0)
 * @param samplePeriod Sample period
 */
VOID mahonyInit(MahonyFilterData *data, FLOAT kp, FLOAT ki, FLOAT samplePeriod);

/**
 * @brief Update Mahony filter without using magnetometer
 *
 * @param data Mahony filter data
 * @param gyroVec Gyrosopce vector in radians per second
 * @param accVec Accelerometer vector in meters per second squared
 */
VOID mahonyUpdateIMU(MahonyFilterData *data, vec3 gyroVec, vec3 accVec);

/**
 * @brief Update Mahony filter using magnetometer
 *
 * @param data Mahony filter data
 * @param gyroVec Gyrosopce vector in radians per second
 * @param accVec Accelerometer vector in meters per second squared
 * @param magVec Magnetometer vector in Gauss
 */
VOID mahonyUpdateMARG(MahonyFilterData *data, vec3 gyroVec, vec3 accVec, vec3 magVec);

/**
 * @brief Internal function for updating Mahony filter
 *
 * @param data Mahony filter data
 * @param gyroVec Gyrosopce vector in radians per second
 * @param halfex Half of the error in x axis
 * @param halfey Half of the error in y axis
 * @param halfez Half of the error in z axis
 */
VOID __mahonyUpdateBase(MahonyFilterData *data, vec3 gyroVec, FLOAT halfex, FLOAT halfey, FLOAT halfez);