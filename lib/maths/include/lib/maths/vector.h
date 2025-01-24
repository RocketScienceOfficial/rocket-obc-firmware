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
 * @brief Add two vectors
 *
 * @param a First vector
 * @param a Second vector
 * @return Result vector
 */
vec3_t vec3_add(const vec3_t *a, const vec3_t *b);

/**
 * @brief Subtract one vector from the other
 *
 * @param a First vector
 * @param a Second vector
 * @return Result vector
 */
vec3_t vec3_sub(const vec3_t *a, const vec3_t *b);

/**
 * @brief Multiply vector by a number
 *
 * @param v Vector to multiply
 * @param n Number
 */
void vec3_mul_num(vec3_t *v, float n);

/**
 * @brief Calculate vector norm (length)
 *
 * @param v Vector
 * @return Vector norm
 */
float vec3_mag(const vec3_t *v);

/**
 * @brief Compare vector magnitude with a value
 *
 * @param v Vector
 * @param n Value to compare magnitude with
 * @return Comparison result
 */
int vec3_mag_compare(const vec3_t *v, float n);

/**
 * @brief Normalize vector
 *
 * @param v Vector to normalize
 */
void vec3_normalize(vec3_t *v);

/**
 * @brief Calculate dot product of two vectors
 *
 * @param a Vector A
 * @param b Vector B
 * @return Dot product
 */
float vec3_dot(const vec3_t *a, const vec3_t *b);

#endif