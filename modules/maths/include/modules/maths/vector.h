#ifndef _VECTOR_H
#define _VECTOR_H

/**
 * @brief 3D Vector
 */
typedef struct vec3
{
    float x, y, z;
} vec3_t;

/**
 * @brief High precision 3D Vector
 */
typedef struct vec3_prec
{
    double x, y, z;
} vec3_prec_t;

/**
 * @brief Calculate vector norm (length)
 *
 * @param v Vector
 * @return Vector norm
 */
float vec3_mag(const vec3_t *v);

/**
 * @brief Normalize vector
 *
 * @param v Vector to normalize
 */
void vec3_normalize(vec3_t *v);

/**
 * @brief Multiply vector by a number
 *
 * @param v Vector to multiply
 * @param n Number
 */
void vec3_mul_num(vec3_t *v, float n);

/**
 * @brief Calculate dot product of two vectors
 *
 * @param a Vector A
 * @param b Vector B
 * @return Dot product
 */
float vec3_dot(const vec3_t *a, const vec3_t *b);

#endif