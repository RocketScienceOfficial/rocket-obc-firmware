#pragma once

#include <obc/api.h>

/**
 * @brief 2D Vector
 */
typedef struct vec2
{
    FLOAT x, y;
} vec2;

/**
 * @brief 3D Vector
 */
typedef struct vec3
{
    FLOAT x, y, z;
} vec3;

/**
 * @brief Add vectors
 *
 * @param a Vector to add to
 * @param b Second vector
 */
VOID vec2Add(vec2 *a, vec2 *b);

/**
 * @brief Subtract vectors
 *
 * @param a Vector to subtract from
 * @param b Second vector
 */
VOID vec2Sub(vec2 *a, vec2 *b);

/**
 * @brief Calculate vector norm (length)
 *
 * @param v Vector
 * @return Vector norm
 */
FLOAT vec2Magnitude(vec2 *v);

/**
 * @brief Normalize vector
 *
 * @param v Vector to normalize
 */
VOID vec2Normalize(vec2 *v);

/**
 * @brief Multiply vector by a number
 *
 * @param n Number
 * @param v Vector to multiply
 */
VOID vec2MulNum(FLOAT n, vec2 *v);

/**
 * @brief Multiply vectors
 *
 * @param res Result of multiplication
 * @param a First vector
 * @param b Second vector
 */
VOID vec2Dot(vec2 *res, vec2 *a, vec2 *b);

/**
 * @brief Calculate vectors dot sum
 *
 * @param a First vector
 * @param b Second vector
 * @return Result of multiplication
 */
FLOAT vec2DotSum(vec2 *a, vec2 *b);

/**
 * @brief Add vectors
 *
 * @param a Vector to add to
 * @param b Second vector
 */
VOID vec3Add(vec3 *a, vec3 *b);

/**
 * @brief Add vectors
 *
 * @param a Vector to subtract from
 * @param b Second vector
 */
VOID vec3Sub(vec3 *a, vec3 *b);

/**
 * @brief Calculate vector norm (length)
 *
 * @param v Vector
 * @return Vector norm
 */
FLOAT vec3Magnitude(vec3 *v);

/**
 * @brief Normalize vector
 *
 * @param v Vector to normalize
 */
VOID vec3Normalize(vec3 *v);

/**
 * @brief Multiply vector by a number
 *
 * @param n Number
 * @param v Vector to multiply
 */
VOID vec3MulNum(FLOAT n, vec3 *v);

/**
 * @brief Multiply vectors
 *
 * @param res Result of multiplication
 * @param a First vector
 * @param b Second vector
 */
VOID vec3Dot(vec3 *res, vec3 *a, vec3 *b);

/**
 * @brief Calculate vectors dot sum
 *
 * @param a First vector
 * @param b Second vector
 * @return Result of multiplication
 */
FLOAT vec3DotSum(vec3 *a, vec3 *b);