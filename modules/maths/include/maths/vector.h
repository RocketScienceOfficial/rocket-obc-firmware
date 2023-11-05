#ifndef _VECTOR_H
#define _VECTOR_H

/**
 * @brief 3D Vector
 */
typedef struct
{
    float x, y, z;
} vec3_t;

/**
 * @brief Add vectors
 *
 * @param a Vector to add to
 * @param b Second vector
 */
void vec3_add(vec3_t *a, vec3_t *b);

/**
 * @brief Add vectors
 *
 * @param a Vector to subtract from
 * @param b Second vector
 */
void vec3_sub(vec3_t *a, vec3_t *b);

/**
 * @brief Calculate vector norm (length)
 *
 * @param v Vector
 * @return Vector norm
 */
float vec3_mag(vec3_t *v);

/**
 * @brief Normalize vector
 *
 * @param v Vector to normalize
 */
void vec3_normalize(vec3_t *v);

/**
 * @brief Multiply vector by a number
 *
 * @param n Number
 * @param v Vector to multiply
 */
void vec3_mul_num(float n, vec3_t *v);

/**
 * @brief Multiply vectors using dot product
 *
 * @param a First vector
 * @param b Second vector
 * @return Sum
 */
float vec3_dot(vec3_t *a, vec3_t *b);

#endif