#ifndef _ADS786X_DRIVER_H
#define _ADS786X_DRIVER_H

#include "hal/spi_driver.h"

/**
 * @brief Type for ADS786X family
 */
typedef enum ads786x_type
{
    ADS786X_TYPE_6,
    ADS786X_TYPE_7,
    ADS786X_TYPE_8,
} ads786x_type_t;

/**
 * @brief ADS786X config structure
 */
typedef struct ads786x_config
{
    hal_spi_instance_t spi;
    hal_pin_number_t cs;
    uint8_t adcBits;
    float vRef;
} ads786x_config_t;

/**
 * @brief Initialize ADS786X device
 *
 * @param config Configuration structure
 * @param spi SPI
 * @param cs CS
 * @param type Type
 * @param vRef Reference voltagse
 */
void ads786x_init(ads786x_config_t *config, hal_spi_instance_t spi, hal_pin_number_t cs, ads786x_type_t type, float vRef);

/**
 * @brief Read ADS786X voltage
 *
 * @param Configuration structure
 * @return Voltage
 */
float ads786x_read(const ads786x_config_t *config);

#endif