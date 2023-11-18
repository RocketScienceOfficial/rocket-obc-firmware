#include "modules/maths/math_utils.h"
#include "modules/maths/math_constants.h"
#include <math.h>

float fast_inv_sqrt(float x)
{
    float xhalf = 0.5f * x;
    long i = *(long *)&x;

    i = 0x5f3759df - (i >> 1);
    x = *(float *)&i;
    x = x * (1.5f - xhalf * x * x);
    x = x * (1.5f - xhalf * x * x);

    return x;
}

float clamp_value(float x, float min, float max)
{
    return x > max ? max : (x < min ? min : x);
}

float height_from_baro_formula(float pressure, float temperature)
{
    return (1 - powf(pressure / SEA_LEVEL_PRESSURE, 0.190284)) * CELSIUS_2_KELVIN(temperature) / STANDARD_LAPSE_RATE;
}