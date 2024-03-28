#include "modules/maths/math_utils.h"

float clamp_value(float x, float min, float max)
{
    return x > max ? max : (x < min ? min : x);
}

bool value_approx_eql(float val, float des, float eps)
{
    return val >= des - eps && val <= des + eps;
}