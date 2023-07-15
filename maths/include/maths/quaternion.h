#pragma once

#include <obc/api.h>
#include "maths/vector.h"

/**
 * @brief Quaternion data structure
 */
typedef struct quat
{
    FLOAT w, x, y, z;
} quat;

/**
 * @brief Add quaternions
 *
 * @param a Quaternion to add to
 * @param b Second quaternion
 */
VOID quatAdd(quat *a, quat *b);

/**
 * @brief Subtract quaternions
 *
 * @param a Quaternion to subtract from
 * @param b Second quaternion
 */
VOID quatSub(quat *a, quat *b);

/**
 * @brief Multiply quaternion by a number
 *
 * @param n Number
 * @param q Quaternion to multiply
 */
VOID quatMulNum(FLOAT n, quat *q);

/**
 * @brief Multiply quaternions
 *
 * @param res Result of multiplication
 * @param a First quaternion
 * @param b Second quaternion
 */
VOID quatMul(quat *res, quat *a, quat *b);

/**
 * @brief Calculate quaternion norm (length)
 *
 * @param q Quaternion
 * @return Quaternion norm
 */
FLOAT quatNorm(quat *q);

/**
 * @brief Calculate conjugate of a quaternion
 *
 * @param q Quaternion to conjugate
 */
VOID quatConjugate(quat *q);

/**
 * @brief Calculate inverse of a quaternion
 *
 * @param q Quaternion to inverse
 */
VOID quatInverse(quat *q);

/**
 * @brief Normalize a quaternion
 *
 * @param q Quaternion to normalize
 */
VOID quatNormalize(quat *q);

/**
 * @brief Convert quaternion to Euler angles
 *
 * @param res Euler angles (in degrees) vector
 * @param q Quaternion
 */
VOID quatToEuler(vec3 *res, quat *q);

/**
 * @brief Rotate vector through quaternion
 *
 * @param v Vector to rotate
 * @param q Quaternion
 */
VOID rotateVectorThroughQuaternion(vec3 *v, quat *q);