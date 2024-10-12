#ifndef _GPS_DRIVER_H
#define _GPS_DRIVER_H

#include "nmea.h"
#include "lib/drivers/utils/gpio_utils.h"
#include "lib/geo/geo.h"
#include "hal/time_tracker.h"
#include <stdbool.h>
#include <stdint.h>

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
 * @brief GPS cache data structure
 */
typedef struct gps_data
{
    geo_position_wgs84_t position;
    float pdop;
    float hdop;
    float vdop;
    float course;
    float speed;
    bool fix;
    bool is3dFix;
    uint8_t activeSatellitesCount;
    uint8_t activeSatellitesIds[24];
    uint8_t _activeSatellitesIndex;
    uint8_t hundredth;
    uint8_t second;
    uint8_t minute;
    uint8_t hour;
    uint8_t day;
    uint8_t month;
    uint8_t year;
} gps_data_t;

/**
 * @brief GPS Configuration
 */
typedef struct gps_config
{
    gpio_utils_communication_config_t gpioConfig; /** GPIO Config */
    gps_nmea_sentence_t sentence;                 /** Sentence data */
    hal_msec_t timeOffset;                        /** Current timer for cooldown  */
    gps_data_t data;                              /** Current cached data */
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

#endif