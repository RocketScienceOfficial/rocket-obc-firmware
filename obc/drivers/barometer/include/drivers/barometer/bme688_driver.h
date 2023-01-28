#pragma once

#include <obc/api.h>
#include "drivers/gpio/gpio_driver.h"
#include "drivers/gpio/spi_driver.h"

/**
 * @brief Barometer config data. All changes are done internally, so keep it just for reference!
 */
typedef struct BME688Config
{
    SPIInstance _spi;
    PinNumber cs;
} BME688Config;

/**
 * @brief Barometer data
 */
typedef struct BME688Data
{
    FLOAT temperature;
    FLOAT humidity;
    INT32 pressure;
} BME688Data;

/**
 * @brief Initialize the barometer
 *
 * @param config Barometer config data
 * @param spi SPI instance.
 * @param miso MISO pin.
 * @param mosi MOSI pin.
 * @param sck SCK pin.
 * @param cs CS pin.
 * @return Result code.
 */
FUNCRESULT bme688Init(BME688Config *config, SPIInstance spi, PinNumber miso, PinNumber mosi, PinNumber sck, PinNumber cs);

/**
 * @brief Checks if the barometer is initialized
 *
 * @param config Barometer config data
 * @param available Is Initialized
 * @return Result code.
 */
FUNCRESULT bme688Check(BME688Config *config, BOOL *result);

/**
 * @brief Read barometer data
 *
 * @param config Barometer config data
 * @param data Returns barometer data
 * @return Result code.
 */
FUNCRESULT bme688Read(BME688Config *config, BME688Data *data);