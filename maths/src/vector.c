#include "maths/vector.h"
#include "maths/math_utils.h"
#include <stdlib.h>
#include <math.h>

VOID vec2Add(vec2 *a, vec2 *b)
{
    a->x += b->x;
    a->y += b->y;
}

VOID vec2Sub(vec2 *a, vec2 *b)
{
    a->x -= b->x;
    a->y -= b->y;
}

FLOAT vec2Magnitude(vec2 *v)
{
    return sqrt(v->x * v->x + v->y * v->y);
}

VOID vec2Normalize(vec2 *v)
{
    FLOAT magnitudeInv = fastInverseSqrt(v->x * v->x + v->y * v->y);

    v->x *= magnitudeInv;
    v->y *= magnitudeInv;
}

VOID vec2MulNum(FLOAT n, vec2 *v)
{
    v->x *= n;
    v->y *= n;
}

VOID vec2Dot(vec2 *res, vec2 *a, vec2 *b)
{
    res->x = a->x * b->x;
    res->y = a->y * b->y;
}

FLOAT vec2DotSum(vec2 *a, vec2 *b)
{
    return a->x * b->x + a->y * b->y;
}

VOID vec3Add(vec3 *a, vec3 *b)
{
    a->x += b->x;
    a->y += b->y;
    a->z += b->z;
}

VOID vec3Sub(vec3 *a, vec3 *b)
{
    a->x -= b->x;
    a->y -= b->y;
    a->z -= b->z;
}

FLOAT vec3Magnitude(vec3 *v)
{
    return sqrt(v->x * v->x + v->y * v->y + v->z * v->z);
}

VOID vec3Normalize(vec3 *v)
{
    FLOAT magnitudeInv = fastInverseSqrt(v->x * v->x + v->y * v->y + v->z * v->z);

    v->x *= magnitudeInv;
    v->y *= magnitudeInv;
    v->z *= magnitudeInv;
}

VOID vec3MulNum(FLOAT n, vec3 *v)
{
    v->x *= n;
    v->y *= n;
    v->z *= n;
}

VOID vec3Dot(vec3 *res, vec3 *a, vec3 *b)
{
    res->x = a->x * b->x;
    res->y = a->y * b->y;
    res->z = a->z * b->z;
}

FLOAT vec3DotSum(vec3 *a, vec3 *b)
{
    return a->x * b->x + a->y * b->y + a->z * b->z;
}