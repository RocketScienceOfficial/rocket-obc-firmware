#include "modules/maths/math_utils.h"
#include "modules/maths/math_constants.h"
#include <math.h>

float fast_inv_sqrt(float x)
{
    long i;
    float x2, y;
    const float threehalfs = 1.5f;

    x2 = x * 0.5f;
    y = x;
    i = *(long *)&y;
    i = 0x5f3759df - (i >> 1);
    y = *(float *)&i;
    y = y * (threehalfs - (x2 * y * y));
    y = y * (threehalfs - (x2 * y * y));

    return y;
}

float clamp_value(float x, float min, float max)
{
    return x > max ? max : (x < min ? min : x);
}

float height_from_baro_formula(float pressure, float temperature)
{
    return (1 - powf(pressure / SEA_LEVEL_PRESSURE, 0.190284f)) * CELSIUS_2_KELVIN(temperature) / STANDARD_LAPSE_RATE;
}

bool value_approx_eql(float val, float des, float eps)
{
    return val >= des - eps && val <= des + eps;
}