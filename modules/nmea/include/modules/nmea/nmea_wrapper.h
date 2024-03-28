#ifndef _NMEA_WRAPPER_H
#define _NMEA_WRAPPER_H

#include "modules/drivers/gps/gps_defines.h"
#include "modules/geo/geo.h"
#include <stdbool.h>

/**
 * @brief Updates data from GNSS
 *
 * @param data Data
 * @param sentence Sentence
 * @return true if was updated
 */
bool gnss_update(geo_position_wgs84_t *data, gps_nmea_sentence_t *sentence);

#endif