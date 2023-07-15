#include "maths/quaternion.h"
#include "maths/constants.h"
#include "maths/math_utils.h"
#include <math.h>

VOID quatAdd(quat *a, quat *b)
{
    a->w += b->w;
    a->x += b->x;
    a->y += b->y;
    a->z += b->z;
}

VOID quatSub(quat *a, quat *b)
{
    a->w -= b->w;
    a->x -= b->x;
    a->y -= b->y;
    a->z -= b->z;
}

VOID quatMulNum(FLOAT n, quat *q)
{
    q->w *= n;
    q->x *= n;
    q->y *= n;
    q->z *= n;
}

VOID quatMul(quat *res, quat *a, quat *b)
{
    res->w = a->w * b->w - a->x * b->x - a->y * b->y - a->z * b->z;
    res->x = a->w * b->x + a->x * b->w + a->y * b->z - a->z * b->y;
    res->y = a->w * b->y - a->x * b->z + a->y * b->w + a->z * b->x;
    res->z = a->w * b->z + a->x * b->y - a->y * b->x + a->z * b->w;
}

FLOAT quatNorm(quat *q)
{
    return sqrt(q->w * q->w + q->x * q->x + q->y * q->y + q->z * q->z);
}

VOID quatConjugate(quat *q)
{
    q->x = -q->x;
    q->y = -q->y;
    q->z = -q->z;
}

VOID quatInverse(quat *q)
{
    FLOAT normInv = fastInverseSqrt(q->w * q->w + q->x * q->x + q->y * q->y + q->z * q->z);
    FLOAT normInv2 = normInv * normInv;

    q->w *= normInv2;
    q->x *= -normInv2;
    q->y *= -normInv2;
    q->z *= -normInv2;
}

VOID quatNormalize(quat *q)
{
    FLOAT normInv = fastInverseSqrt(q->w * q->w + q->x * q->x + q->y * q->y + q->z * q->z);

    q->w *= normInv;
    q->x *= normInv;
    q->y *= normInv;
    q->z *= normInv;
}

VOID quatToEuler(vec3 *res, quat *q)
{
    res->x = RAD_2_DEG(atan2(2 * (q->w * q->x + q->y * q->z), 1 - 2 * (q->x * q->x + q->y * q->y)));
    res->y = RAD_2_DEG(asin(clampValue(2 * (q->w * q->y - q->x * q->z), -1, 1)));
    res->z = RAD_2_DEG(atan2(2 * (q->w * q->z + q->x * q->y), 1 - 2 * (q->y * q->y + q->z * q->z)));
}

VOID rotateVectorThroughQuaternion(vec3 *v, quat *q)
{
    *v = (vec3){
        .x = v->x * (1 - 2 * (q->y * q->y + q->z * q->z)) + v->y * 2 * (q->x * q->y - q->z * q->w) + v->z * 2 * (q->x * q->z + q->y * q->w),
        .y = v->x * 2 * (q->x * q->y + q->z * q->w) + v->y * (1 - 2 * (q->x * q->x + q->z * q->z)) + v->z * 2 * (q->y * q->z - q->x * q->w),
        .z = v->x * 2 * (q->x * q->z - q->y * q->w) + v->y * 2 * (q->y * q->z + q->x * q->w) + v->z * (1 - 2 * (q->x * q->x + q->y * q->y)),
    };
}