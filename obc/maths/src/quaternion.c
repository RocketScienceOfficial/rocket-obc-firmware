#include "maths/quaternion.h"
#include "maths/constants.h"
#include "maths/math_utils.h"
#include <math.h>

VOID quatAdd(quat *a, quat *b)
{
    a->q1 += b->q1;
    a->q2 += b->q2;
    a->q3 += b->q3;
    a->q4 += b->q4;
}

VOID quatSub(quat *a, quat *b)
{
    a->q1 -= b->q1;
    a->q2 -= b->q2;
    a->q3 -= b->q3;
    a->q4 -= b->q4;
}

VOID quatMulNum(FLOAT n, quat *q)
{
    q->q1 *= n;
    q->q2 *= n;
    q->q3 *= n;
    q->q4 *= n;
}

VOID quatMul(quat *res, quat *a, quat *b)
{
    res->q1 = a->q1 * b->q1 - a->q2 * b->q2 - a->q3 * b->q3 - a->q4 * b->q4;
    res->q2 = a->q1 * b->q2 + a->q2 * b->q1 + a->q3 * b->q4 - a->q4 * b->q3;
    res->q3 = a->q1 * b->q3 - a->q2 * b->q4 + a->q3 * b->q1 + a->q4 * b->q2;
    res->q4 = a->q1 * b->q4 + a->q2 * b->q3 - a->q3 * b->q2 + a->q4 * b->q1;
}

FLOAT quatNorm(quat *q)
{
    return sqrt(q->q1 * q->q1 + q->q2 * q->q2 + q->q3 * q->q3 + q->q4 * q->q4);
}

VOID quatConjugate(quat *q)
{
    q->q2 = -q->q2;
    q->q3 = -q->q3;
    q->q4 = -q->q4;
}

VOID quatInverse(quat *q)
{
    FLOAT normInv = fastInverseSqrt(q->q1 * q->q1 + q->q2 * q->q2 + q->q3 * q->q3 + q->q4 * q->q4);
    FLOAT normInv2 = normInv * normInv;

    q->q1 *= normInv2;
    q->q2 *= -normInv2;
    q->q3 *= -normInv2;
    q->q4 *= -normInv2;
}

VOID quatNormalize(quat *q)
{
    FLOAT normInv = fastInverseSqrt(q->q1 * q->q1 + q->q2 * q->q2 + q->q3 * q->q3 + q->q4 * q->q4);

    q->q1 *= normInv;
    q->q2 *= normInv;
    q->q3 *= normInv;
    q->q4 *= normInv;
}

VOID quatToEuler(vec3 *res, quat *q)
{
    res->x = RAD_2_DEG(atan2(2 * (q->q1 * q->q2 + q->q3 * q->q4), 1 - 2 * (q->q2 * q->q2 + q->q3 * q->q3)));
    res->y = RAD_2_DEG(asin(clampValue(2 * (q->q1 * q->q3 - q->q2 * q->q4), -1, 1)));
    res->z = RAD_2_DEG(atan2(2 * (q->q1 * q->q4 + q->q2 * q->q3), 1 - 2 * (q->q3 * q->q3 + q->q4 * q->q4)));
}