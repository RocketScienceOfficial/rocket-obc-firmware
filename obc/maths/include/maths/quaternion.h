#pragma once

#include <obc/api.h>
#include "maths/vector.h"

typedef union quat
{
    struct
    {
        vec4 v;
    };

    struct
    {
        FLOAT a, b, c, d;
    };
} quat;

quat quatAdd(quat a, quat b);
quat quatSub(quat a, quat b);
quat quatMul(quat a, quat b);