#ifndef _NMEA_WRAPPER_H
#define _NMEA_WRAPPER_H

#include "nmea_parser.h"
#include "modules/drivers/gps/gps_defines.h"
#include <stdbool.h>

/**
 * @brief Structure for GNSS data
 */
typedef struct gnss_data
{
    nmea_lat_lon_t lat; /** Latitude */
    nmea_lat_lon_t lon; /** Longitude */
    float alt;          /** Altitude */
} gnss_data_t;

/**
 * @brief Updates data from GNSS
 *
 * @param data Data
 * @param sentence Sentence
 * @return true if was updated
 */
bool gnss_update(gnss_data_t *data, gps_nmea_sentence_t *sentence);

#endif