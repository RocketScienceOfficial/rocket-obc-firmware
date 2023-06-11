#include "maths/math_utils.h"
#include "maths/constants.h"
#include <math.h>
#include <stdlib.h>

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
    if (value < min)
    {
        return min;
    }
    else if (value > max)
    {
        return max;
    }
    else
    {
        return value;
    }
}

FLOAT heightFromBarometricFormula(FLOAT pressure, FLOAT temperature)
{
    return (1 - pow(pressure / 101325, 0.190284)) * (temperature + CELSIUS_KELVIN) / 0.0065;
}

FLOAT distanceBetweenGeoPoints(FLOAT lat1, FLOAT lon1, FLOAT lat2, FLOAT lon2)
{
    FLOAT dLat = DEG_2_RAD(lat2 - lat1);
    FLOAT dLon = DEG_2_RAD(lon2 - lon1);
    FLOAT a = sin(dLat / 2) * sin(dLat / 2) + cos(DEG_2_RAD(lat1)) * cos(DEG_2_RAD(lat2)) * sin(dLon / 2) * sin(dLon / 2);
    FLOAT c = 2 * atan2(sqrt(a), sqrt(1 - a));

    return EARTH_RADIUS * c;
}

FLOAT bearingBetweenGeoPoints(FLOAT lat1, FLOAT lon1, FLOAT lat2, FLOAT lon2)
{
    FLOAT dLon = DEG_2_RAD(lon2 - lon1);
    FLOAT y = sin(dLon) * cos(DEG_2_RAD(lat2));
    FLOAT x = cos(DEG_2_RAD(lat1)) * sin(DEG_2_RAD(lat2)) - sin(DEG_2_RAD(lat1)) * cos(DEG_2_RAD(lat2)) * cos(dLon);
    FLOAT bearing = RAD_2_DEG(atan2(y, x));

    return fmod(bearing + 360, 360);
}