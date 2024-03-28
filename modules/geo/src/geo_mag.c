#include "modules/geo/geo_mag.h"
#include "modules/geo/generated/mag_tables.h"
#include <stdint.h>
#include <math.h>

static float _get_value_from_table(geo_position_t pos, const int16_t table[LAT_DIM][LON_DIM]);

float geo_mag_get_declination(geo_position_t pos)
{
    return _get_value_from_table(pos, DECLINATION_TABLE);
}

float geo_mag_get_inclination(geo_position_t pos)
{
    return _get_value_from_table(pos, INCLINATION_TABLE);
}

float geo_mag_get_strength(geo_position_t pos)
{
    return _get_value_from_table(pos, STRENGTH_TABLE);
}

float _get_value_from_table(geo_position_t pos, const int16_t table[LAT_DIM][LON_DIM])
{
    float min_lat = floor(pos.lat / SAMPLING_RES) * SAMPLING_RES;
    float min_lon = floor(pos.lat / SAMPLING_RES) * SAMPLING_RES;

    int min_lat_index = (int)((min_lat - SAMPLING_MIN_LAT) / SAMPLING_RES);
    int min_lon_index = (int)((min_lon - SAMPLING_MIN_LON) / SAMPLING_RES);

    float data_ne = (float)table[min_lat_index + 1][min_lon_index + 1];
    float data_se = (float)table[min_lat_index][min_lon_index + 1];
    float data_sw = (float)table[min_lat_index][min_lon_index];
    float data_nw = (float)table[min_lat_index + 1][min_lon_index];

    float data_min = (data_se - data_sw) / SAMPLING_RES * (pos.lon - min_lon) + data_sw;
    float data_max = (data_ne - data_nw) / SAMPLING_RES * (pos.lon - min_lon) + data_nw;
    float data = (data_max - data_min) / SAMPLING_RES * (pos.lat - min_lat) + data_min;

    return data / 10000.0f;
}