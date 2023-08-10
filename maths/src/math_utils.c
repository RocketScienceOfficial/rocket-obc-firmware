#include "maths/math_utils.h"
#include "maths/constants.h"
#include <math.h>

FLOAT fastInverseSqrt(FLOAT x)
{
    FLOAT xhalf = 0.5f * x;
    INT32 i = *(INT32 *)&x;

    i = 0x5f3759df - (i >> 1);
    x = *(FLOAT *)&i;
    x = x * (1.5f - xhalf * x * x);
    x = x * (1.5f - xhalf * x * x);

    return x;
}

FLOAT clampValue(FLOAT value, FLOAT min, FLOAT max)
{
    return value < min ? min : value > max ? max : value;
}

FLOAT heightFromBarometricFormula(FLOAT pressure, FLOAT temperature)
{
    return (1 - pow(pressure / SEA_LEVEL_PRESSURE, 0.190284)) * CELSIUS_2_KELVIN(temperature) / STANDARD_LAPSE_RATE;
}