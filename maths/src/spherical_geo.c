#include "maths/spherical_geo.h"
#include "maths/constants.h"
#include <math.h>

FLOAT calculateDistanceBetweenGeoPoints(FLOAT lat1, FLOAT lon1, FLOAT lat2, FLOAT lon2)
{
    FLOAT dLat = DEG_2_RAD(lat2 - lat1);
    FLOAT dLon = DEG_2_RAD(lon2 - lon1);
    FLOAT a = sin(dLat / 2) * sin(dLat / 2) + cos(DEG_2_RAD(lat1)) * cos(DEG_2_RAD(lat2)) * sin(dLon / 2) * sin(dLon / 2);
    FLOAT c = 2 * atan2(sqrt(a), sqrt(1 - a));

    return EARTH_RADIUS * c;
}

FLOAT calculateHaversineFormula(FLOAT lat1, FLOAT lon1, FLOAT lat2, FLOAT lon2)
{
    return EARTH_RADIUS * atan2f(sqrtf(powf(cosf(lat2) * sinf(lon2 - lon1), 2) + powf(cosf(lat1) * sinf(lat2) - sinf(lat1) * cosf(lat2) * cosf(lon2 - lon1), 2)), sinf(lat1) * sinf(lat2) + cosf(lat1) * cosf(lat2) * cosf(lon2 - lon1));
}

FLOAT calculateBearingAngleBetweenGeoPoints(FLOAT lat1, FLOAT lon1, FLOAT lat2, FLOAT lon2)
{
    FLOAT dLon = DEG_2_RAD(lon2 - lon1);
    FLOAT y = sin(dLon) * cos(DEG_2_RAD(lat2));
    FLOAT x = cos(DEG_2_RAD(lat1)) * sin(DEG_2_RAD(lat2)) - sin(DEG_2_RAD(lat1)) * cos(DEG_2_RAD(lat2)) * cos(dLon);
    FLOAT bearing = RAD_2_DEG(atan2(y, x));

    return fmod(bearing + 360, 360);
}

vec2 calculateTriangleSidesFromGeoPoints(FLOAT lat1, FLOAT lon1, FLOAT lat2, FLOAT lon2)
{
    FLOAT dLat = DEG_2_RAD(lat2 - lat1);
    FLOAT dLon = DEG_2_RAD(lon2 - lon1);
    FLOAT a = sin(dLat / 2) * sin(dLat / 2) + cos(DEG_2_RAD(lat1)) * cos(DEG_2_RAD(lat2)) * sin(dLon / 2) * sin(dLon / 2);
    FLOAT c = 2 * atan2(sqrt(a), sqrt(1 - a));
    FLOAT d = EARTH_RADIUS * c;

    return (vec2){d * cos(DEG_2_RAD(lat1)) * sin(dLon), d * sin(DEG_2_RAD(lat1)) * cos(DEG_2_RAD(lat2)) - sin(DEG_2_RAD(lat1)) * cos(DEG_2_RAD(lat2)) * cos(dLon)};
}