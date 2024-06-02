#include "lib/geo/geo_utils.h"
#include "lib/geo/physical_constants.h"
#include <math.h>

#define EXP_CONSTANT 0.1902632

float height_from_baro_formula(int pressure)
{
    return SEA_LEVEL_TEMPERATURE / STANDARD_LAPSE_RATE * (1 - pow(pressure / (double)SEA_LEVEL_PRESSURE, EXP_CONSTANT));
}