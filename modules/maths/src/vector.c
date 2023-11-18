#include "modules/maths/vector.h"
#include "modules/maths/math_utils.h"
#include <math.h>

void vec3_add(vec3_t *a, vec3_t *b)
{
    a->x += b->x;
    a->y += b->y;
    a->z += b->z;
}

void vec3_sub(vec3_t *a, vec3_t *b)
{
    a->x -= b->x;
    a->y -= b->y;
    a->z -= b->z;
}

float vec3_mag(vec3_t *v)
{
    return sqrtf(v->x * v->x + v->y * v->y + v->z * v->z);
}

void vec3_normalize(vec3_t *v)
{
    float magInv = fast_inv_sqrt(v->x * v->x + v->y * v->y + v->z * v->z);

    v->x *= magInv;
    v->y *= magInv;
    v->z *= magInv;
}

void vec3_mul_num(float n, vec3_t *v)
{
    v->x *= n;
    v->y *= n;
    v->z *= n;
}

float vec3_dot(vec3_t *a, vec3_t *b)
{
    return a->x * b->x + a->y * b->y + a->z * b->z;
}