#ifndef _GPS_DRIVER_H
#define _GPS_DRIVER_H

#include "lib/drivers/utils/gpio_utils.h"
#include "lib/geo/geo.h"
#include "lib/nmea/nmea_parser.h"
#include <stdbool.h>
#include <stddef.h>

/**
 * @brief Structure for holding gps nmea sentence
 */
typedef struct gps_nmea_sentence
{
    char data[NMEA_SENTENCE_MAX_LENGTH]; /** Sentence string with null terminator */
    size_t currentIndex;                 /** Current index of data */
    bool isFinished;                     /** Is sentence finished */
} gps_nmea_sentence_t;

/**
 * @brief GPS Configuration
 */
typedef struct gps_config
{
    gpio_utils_communication_config_t gpioConfig; /** GPIO Config */
    gps_nmea_sentence_t sentence;                 /** Sentence data */
} gps_config_t;

/**
 * @brief Initializes the GPS module
 *
 * @param config The configuration of the GPS module
 * @param spi The SPI instance to use
 * @param cs The CS pin to use
 */
void gps_init_spi(gps_config_t *config, hal_spi_instance_t spi, hal_pin_number_t cs);

/**
 * @brief Reads a NMEA sentence from the GPS module
 *
 * @param config The configuration of the GPS module
 * @return True if next read is available
 */
bool gps_read(gps_config_t *config);

/**
 * @brief Reads position from cached NMEA sentence and overwrites provied position
 *
 * @param config The configuration of the GPS module
 * @param pos Position to overwrite
 * @return True if position was overwritten
 */
bool gps_get_pos(gps_config_t *config, geo_position_wgs84_t *pos);

#endif