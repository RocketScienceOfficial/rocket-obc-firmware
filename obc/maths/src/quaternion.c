#include "maths/quaternion.h"
#include "maths/constants.h"
#include <math.h>

quat quatAdd(quat a, quat b)
{
    return (quat){.q1 = a.q1 + b.q1, .q2 = a.q2 + b.q2, .q3 = a.q3 + b.q3, .q4 = a.q4 + b.q4};
}

quat quatSub(quat a, quat b)
{
    return (quat){.q1 = a.q1 - b.q1, .q2 = a.q2 - b.q2, .q3 = a.q3 - b.q3, .q4 = a.q4 - b.q4};
}

quat quatMulNum(FLOAT n, quat q)
{
    return (quat){.q1 = n * q.q1, .q2 = n * q.q2, .q3 = n * q.q3, .q4 = n * q.q4};
}

quat quatMul(quat a, quat b)
{
    return (quat){
        .q1 = a.q1 * b.q1 - a.q2 * b.q2 - a.q3 * b.q3 - a.q4 * b.q4,
        .q2 = a.q1 * b.q2 + a.q2 * b.q1 + a.q3 * b.q4 - a.q4 * b.q3,
        .q3 = a.q1 * b.q3 - a.q2 * b.q4 + a.q3 * b.q1 + a.q4 * b.q2,
        .q4 = a.q1 * b.q4 + a.q2 * b.q3 - a.q3 * b.q2 + a.q4 * b.q1,
    };
}

FLOAT quatNorm(quat q)
{
    return sqrt(q.q1 * q.q1 + q.q2 * q.q2 + q.q3 * q.q3 + q.q4 * q.q4);
}

quat quatConjugate(quat q)
{
    return (quat){
        .q1 = q.q1,
        .q2 = -q.q2,
        .q3 = -q.q3,
        .q4 = -q.q4,
    };
}

quat quatInverse(quat q)
{
    FLOAT norm = quatNorm(q);

    return quatMulNum(1 / (norm * norm), quatConjugate(q));
}

quat quatNormalize(quat q)
{
    return quatMulNum(1 / quatNorm(q), q);
}

vec3 quatToEuler(quat q)
{
    return (vec3){
        .x = RAD_2_DEG(atan2(2 * (q.q1 * q.q2 + q.q3 * q.q4), 1 - 2 * (q.q2 * q.q2 + q.q3 * q.q3))),
        .y = RAD_2_DEG(asin(2 * (q.q1 * q.q3 - q.q4 * q.q2))),
        .z = RAD_2_DEG(atan2(2 * (q.q1 * q.q4 + q.q2 * q.q3), 1 - 2 * (q.q3 * q.q3 + q.q4 * q.q4))),
    };
}