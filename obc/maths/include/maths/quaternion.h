#pragma once

#include <obc/api.h>
#include "maths/vector.h"

/**
 * @brief Quaternion data structure.
*/
typedef union quat
{
    struct
    {
        FLOAT q1, q2, q3, q4;
    };

    struct
    {
        FLOAT w, x, y, z;
    };
} quat;

/**
 * @brief Add quaternions.
 * 
 * @param a First quaternion.
 * @param b Second quaternion.
 * @return Result of addition.
*/
quat quatAdd(quat a, quat b);

/**
 * @brief Subtract quaternions.
 * 
 * @param a First quaternion.
 * @param b Second quaternion.
 * @return Result of subtraction.
*/
quat quatSub(quat a, quat b);

/**
 * @brief Multiply quaternion by a number.
 * 
 * @param n Number.
 * @param q Quaternion.
 * @return Result of multiplication.
*/
quat quatMulNum(FLOAT n, quat q);

/**
 * @brief Multiply quaternions.
 * 
 * @param a First quaternion.
 * @param b Second quaternion.
 * @return Result of multiplication.
*/
quat quatMul(quat a, quat b);

/**
 * @brief Calculate quaternion norm (length).
 * 
 * @param q Quaternion.
 * @return Quaternion norm.
*/
FLOAT quatNorm(quat q);

/**
 * @brief Calculate conjugate of a quaternion.
 * 
 * @param q Quaternion.
 * @return Conjugate of the quaternion.
*/
quat quatConjugate(quat q);

/**
 * @brief Calculate inverse of a quaternion.
 * 
 * @param q Quaternion.
 * @return Inverse of the quaternion.
*/
quat quatInverse(quat q);

/**
 * @brief Normalize a quaternion.
 * 
 * @param q Quaternion.
 * @return Normalized quaternion.
*/
quat quatNormalize(quat q);

/**
 * @brief Convert quaternion to Euler angles.
 * 
 * @param q Quaternion.
 * @return Euler angles vector.
*/
vec3 quatToEuler(quat q);