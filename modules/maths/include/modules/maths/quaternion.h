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
 * @brief Add quaternions
 *
 * @param a Quaternion to add to
 * @param b Second quaternion
 */
void quat_add(quat_t *a, quat_t *b);

/**
 * @brief Subtract quaternions
 *
 * @param a Quaternion to subtract from
 * @param b Second quaternion
 */
void quat_sub(quat_t *a, quat_t *b);

/**
 * @brief Multiply quaternion by a number
 *
 * @param n Number
 * @param q Quaternion to multiply
 */
void quat_mul_num(float n, quat_t *q);

/**
 * @brief Multiply quaternions
 *
 * @param res Result of multiplication
 * @param a First quaternion
 * @param b Second quaternion
 */
void quat_mul(quat_t *res, quat_t *a, quat_t *b);

/**
 * @brief Calculate quaternion norm (length)
 *
 * @param q Quaternion
 * @return Quaternion norm
 */
float quat_norm(quat_t *q);

/**
 * @brief Calculate conjugate of a quaternion
 *
 * @param q Quaternion to conjugate
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
 * @brief Convert quaternion to Euler angles
 *
 * @param res Euler angles (in degrees) vector
 * @param q Quaternion
 */
void quat_to_euler(vec3_t *res, quat_t *q);

/**
 * @brief Rotate vector through quaternion
 *
 * @param v Vector to rotate
 * @param q Quaternion
 */
void rotate_vec_through_quat(vec3_t *v, quat_t *q);

#endif