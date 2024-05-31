#ifndef _ADS7038_DRIVER_H
#define _ADS7038_DRIVER_H

#include "hal/spi_driver.h"
#include <stdint.h>

/**
 * @brief ADS7038 channel type
 */
typedef enum ads7038_channel_type
{
    ADS7038_CHANNEL_AIN0 = 1 << 0,
    ADS7038_CHANNEL_AIN1 = 1 << 1,
    ADS7038_CHANNEL_AIN2 = 1 << 2,
    ADS7038_CHANNEL_AIN3 = 1 << 3,
    ADS7038_CHANNEL_AIN4 = 1 << 4,
    ADS7038_CHANNEL_AIN5 = 1 << 5,
    ADS7038_CHANNEL_AIN6 = 1 << 6,
    ADS7038_CHANNEL_AIN7 = 1 << 7,
} ads7038_channel_type_t;

/**
 * @brief ADS7038 configuration structure
 */
typedef struct ads7038_config
{
    hal_spi_instance_t spi;
    hal_pin_number_t cs;
    float vRef;
} ads7038_config_t;

/**
 * @brief Initialize ADS7038 device
 *
 * @param config Configuration structure
 * @param spi SPI
 * @param cs CS
 * @param channelsMask Channels combined in one byte (ads7038_channel_type_t)
 * @param vRef Reference voltage
 */
void ads7038_init(ads7038_config_t *config, hal_spi_instance_t spi, hal_pin_number_t cs, uint8_t channelsMask, float vRef);

/**
 * @brief Read selected channels from ADS7038 device
 *
 * @param config Configuration structure
 * @param values Voltage levels for each channel
 * @param len Length of values (channels)
 */
void ads7038_read_channels(const ads7038_config_t *config, float *values, size_t len);

#endif