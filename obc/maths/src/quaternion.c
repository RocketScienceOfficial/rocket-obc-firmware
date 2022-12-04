#include "maths/quaternion.h"

quat quatAdd(quat a, quat b)
{
    return (quat){.v = vec4Add(a.v, b.v)};
}

quat quatSub(quat a, quat b)
{
    return (quat){.v = vec4Sub(a.v, b.v)};
}

quat quatMul(quat a, quat b)
{
    return (quat){
        .a = a.a * b.a - a.b * b.b - a.c * b.c - a.d * b.d,
        .b = a.a * b.b + a.b * b.a + a.c * b.d - a.d * b.c,
        .c = a.a * b.c - a.b * b.d + a.c * b.a + a.d * b.b,
        .d = a.a * b.d + a.b * b.c - a.c * b.b + a.d * b.a,
    };
}