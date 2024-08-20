#ifndef _QUATERNION_H
#define _QUATERNION_H

#include "vector.h"

/**
 * @brief Quaternion data structure
 */
typedef struct quat
{
    float w, x, y, z;
} quat_t;

/**
 * @brief Multiply quaternions
 *
 * @param a First quaternion
 * @param b Second quaternion
 * @return Result of multiplication
 */
quat_t quat_mul(const quat_t *a, const quat_t *b);

/**
 * @brief Calculate conjugate of quaternion
 *
 * @param q Quaternion
 */
void quat_conj(quat_t *q);

/**
 * @brief Calculate inverse of a quaternion
 *
 * @param q Quaternion to inverse
 */
void quat_inv(quat_t *q);

/**
 * @brief Normalize a quaternion
 *
 * @param q Quaternion to normalize
 */
void quat_normalize(quat_t *q);

/**
 * @brief Rotate vector through quaternion
 *
 * @param v Vector to rotate
 * @param q Quaternion
 */
void rotate_vec_through_quat(vec3_t *v, const quat_t *q);

#endif