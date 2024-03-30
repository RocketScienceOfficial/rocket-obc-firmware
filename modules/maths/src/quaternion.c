#include "modules/maths/quaternion.h"
#include "modules/maths/math_constants.h"
#include "modules/maths/fast_math.h"
#include "modules/maths/math_utils.h"
#include <math.h>

quat_t quat_mul(const quat_t *a, const quat_t *b)
{
    return (quat_t){
        .w = a->w * b->w - a->x * b->x - a->y * b->y - a->z * b->z,
        .x = a->w * b->x + a->x * b->w + a->y * b->z - a->z * b->y,
        .y = a->w * b->y - a->x * b->z + a->y * b->w + a->z * b->x,
        .z = a->w * b->z + a->x * b->y - a->y * b->x + a->z * b->w,
    };
}

void quat_conj(quat_t *q)
{
    q->x = -q->x;
    q->y = -q->y;
    q->z = -q->z;
}

void quat_inv(quat_t *q)
{
    float normInv = fast_inv_sqrt(q->w * q->w + q->x * q->x + q->y * q->y + q->z * q->z);
    float normInv2 = normInv * normInv;

    q->w *= normInv2;
    q->x *= -normInv2;
    q->y *= -normInv2;
    q->z *= -normInv2;
}

void quat_normalize(quat_t *q)
{
    float normInv = fast_inv_sqrt(q->w * q->w + q->x * q->x + q->y * q->y + q->z * q->z);

    q->w *= normInv;
    q->x *= normInv;
    q->y *= normInv;
    q->z *= normInv;
}

vec3_t quat_to_euler(const quat_t *q)
{
    return (vec3_t){
        .x = RAD_2_DEG(atan2f(2.0f * (q->w * q->x + q->y * q->z), 1.0f - 2.0f * (q->x * q->x + q->y * q->y))),
        .y = RAD_2_DEG(asinf(clamp_value(2.0f * (q->w * q->y - q->x * q->z), -1.0f, 1.0f))),
        .z = RAD_2_DEG(atan2f(2.0f * (q->w * q->z + q->x * q->y), 1.0f - 2.0f * (q->y * q->y + q->z * q->z))),
    };
}

void rotate_vec_through_quat(vec3_t *v, const quat_t *q)
{
    *v = (vec3_t){
        .x = v->x * (1.0f - 2.0f * (q->y * q->y + q->z * q->z)) + v->y * 2.0f * (q->x * q->y - q->z * q->w) + v->z * 2.0f * (q->x * q->z + q->y * q->w),
        .y = v->x * 2.0f * (q->x * q->y + q->z * q->w) + v->y * (1.0f - 2.0f * (q->x * q->x + q->z * q->z)) + v->z * 2.0f * (q->y * q->z - q->x * q->w),
        .z = v->x * 2.0f * (q->x * q->z - q->y * q->w) + v->y * 2.0f * (q->y * q->z + q->x * q->w) + v->z * (1.0f - 2.0f * (q->x * q->x + q->y * q->y)),
    };
}