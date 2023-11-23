#ifndef _U_BLOX_NEO_M9N_DRIVER_H
#define _U_BLOX_NEO_M9N_DRIVER_H

#include "modules/drivers/utils/gpio_utils.h"
#include <stdbool.h>
#include <stddef.h>

#define UBLOX_NEO_M9N_SENTENCE_LENGTH 83

/**
 * @brief The configuration of the u-blox NEO-M9N GPS module.
 */
typedef struct ublox_neo_m9n_config
{
    hal_gpio_communication_config_t gpioConfig; /** The GPIO communication configuration */
} ublox_neo_m9n_config_t;

/**
 * @brief The data of the u-blox NEO-M9N GPS module.
 */
typedef struct ublox_neo_m9n_data
{
    bool isValid;                                 /** Is current data valid */
    char sentence[UBLOX_NEO_M9N_SENTENCE_LENGTH]; /** Sentence buffer */
    size_t currentIndex;                          /** Current index of buffer */
    bool isFinishedSentence;                      /** Is current sentence ready */
} ublox_neo_m9n_data_t;

/**
 * @brief Initializes the u-blox NEO-M9N GPS module.
 *
 * @param config The configuration of the u-blox NEO-M9N GPS module.
 * @param spi The SPI instance to use.
 * @param miso The MISO pin to use.
 * @param mosi The MOSI pin to use.
 * @param cs The CS pin to use.
 * @param sck The SCK pin to use.
 */
void ublox_neo_m9n_init_spi(ublox_neo_m9n_config_t *config, hal_spi_instance_t spi, hal_pin_number_t miso, hal_pin_number_t mosi, hal_pin_number_t cs, hal_pin_number_t sck);

/**
 * @brief Initializes the u-blox NEO-M9N GPS module with I2C
 *
 * @param config The configuration of the u-blox NEO-M9N GPS module
 * @param i2c The I2C instance to use
 * @param sda The SDA pin to use
 * @param scl The SCL pin to use
 */
void ublox_neo_m9n_init_i2c(ublox_neo_m9n_config_t *config, hal_i2c_instance_t i2c, hal_pin_number_t sda, hal_pin_number_t scl);

/**
 * @brief Reads a NMEA sentence from the u-blox NEO-M9N GPS module.
 *
 * @param config The configuration of the u-blox NEO-M9N GPS module.
 * @param data The data of the u-blox NEO-M9N GPS module.
 */
void ublox_neo_m9n_read_data(ublox_neo_m9n_config_t *config, ublox_neo_m9n_data_t *data);

#endif