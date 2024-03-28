#include "modules/maths/vector.h"
#include "modules/maths/fast_math.h"
#include <math.h>

float vec3_mag(const vec3_t *v)
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

void vec3_mul_num(vec3_t *v, float n)
{
    v->x *= n;
    v->y *= n;
    v->z *= n;
}

float vec3_dot(const vec3_t *a, const vec3_t *b)
{
    return a->x * b->x + a->y * b->y + a->z * b->z;
}