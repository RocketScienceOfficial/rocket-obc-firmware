#include "lib/geo/geo_utils.h"
#include "lib/geo/physical_constants.h"
#include <math.h>

float height_from_baro_formula(float pressure, float temperature)
{
    return (1 - powf(pressure / SEA_LEVEL_PRESSURE, 0.190284f)) * CELSIUS_2_KELVIN(temperature) / STANDARD_LAPSE_RATE;
}