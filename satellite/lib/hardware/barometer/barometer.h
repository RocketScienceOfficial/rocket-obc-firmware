#pragma once

#include "errors_types.h"
#include <stdio.h>
#include <stdbool.h>

#define PRESSURE_TEMPERATURE_CONSTANT 44330.769
#define PRESSURE_GAS_CONSTANT 0.19026
#define SEA_LEVEL_PRESSURE 101325.0

/**
 * @brief Barometer data
 */
typedef struct BarometerData
{
    float temperature;
    int pressure;
} BarometerData;

/**
 * @brief Barometer config data. All changes are done internally, so keep it just for reference!
 */
typedef struct BarometerConfig
{
    int _i2c;
} BarometerConfig;

/**
 * @brief Initialize the barometer
 *
 * @param config Barometer config data
 * @param i2c I2C (0 or 1)
 * @param sda SDA pin
 * @param scl SCL pin
 * @return Result code.
 */
FUNCRESULT barometerInit(BarometerConfig *config, int i2c, int sda, int scl);

/**
 * @brief Checks if the barometer is initialized
 *
 * @param config Barometer config data
 * @param available Is Initialized
 * @return Result code.
 */
FUNCRESULT barometerCheck(BarometerConfig *config, bool *result);

/**
 * @brief Read barometer data
 *
 * @param config Barometer config data
 * @param data Returns barometer data
 * @return Result code.
 */
FUNCRESULT barometerRead(BarometerConfig *config, BarometerData *data);

/**
 * @brief Calculate altitude (in m) from pressure
 *
 * @param config Barometer config data
 * @param data Barometer data
 * @param altitude Returns altitude (in m)
 * @return Result code.
 */
FUNCRESULT barometerGetAltitude(BarometerConfig *config, BarometerData *data, float *altitude);