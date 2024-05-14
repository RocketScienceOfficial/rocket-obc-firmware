#ifndef _GEO_MAG_H
#define _GEO_MAG_H

#include "geo.h"

/**
 * @brief Get magnetic declination at location
 *
 * @param pos Coordinates
 * @return Declination in radians
 */
float geo_mag_get_declination(geo_position_t pos);

/**
 * @brief Get magnetic inclination at location
 *
 * @param pos Coordinates
 * @return Inclination in radians
 */
float geo_mag_get_inclination(geo_position_t pos);

/**
 * @brief Get magnetic strength at location
 *
 * @param pos Coordinates
 * @return Strength in Gauss
 */
float geo_mag_get_strength(geo_position_t pos);

#endif