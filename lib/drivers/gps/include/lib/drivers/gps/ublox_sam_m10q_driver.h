#ifndef _U_BLOX_SAM_M10Q_DRIVER_H
#define _U_BLOX_SAM_M10Q_DRIVER_H

#include "gps_defines.h"
#include "lib/drivers/utils/gpio_utils.h"
#include <stddef.h>

/**
 * @brief The configuration of the u-blox SAM-M10Q GPS module
 */
typedef struct ublox_sam_m10q_config
{
    hal_gpio_communication_config_t gpioConfig; /** The GPIO communication configuration */
} ublox_sam_m10q_config_t;

/**
 * @brief Initializes the u-blox SAM-M10Q GPS module with SPI
 *
 * @param config The configuration of the u-blox SAM-M10Q GPS module
 * @param spi The SPI instance to use
 * @param cs The CS pin to use
 */
void ublox_sam_m10q_init_spi(ublox_sam_m10q_config_t *config, hal_spi_instance_t spi, hal_pin_number_t cs);

/**
 * @brief Initializes the u-blox SAM-M10Q GPS module with I2C
 *
 * @param config The configuration of the u-blox SAM-M10Q GPS module
 * @param i2c The I2C instance to use
 */
void ublox_sam_m10q_init_i2c(ublox_sam_m10q_config_t *config, hal_i2c_instance_t i2c);

/**
 * @brief Reads a NMEA sentence from the u-blox SAM-M10Q GPS module
 *
 * @param config The configuration of the u-blox SAM-M10Q GPS module
 * @param sentence Sentence data
 */
void ublox_sam_m10q_read_data(ublox_sam_m10q_config_t *config, gps_nmea_sentence_t *sentence);

#endif