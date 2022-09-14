#pragma once

#include "drivers/hwutils/errors_types.h"
#include <stdbool.h>
#include <stdint.h>

/**
 * @brief Barometer data
 */
typedef struct BMP280Data
{
    float temperature;
    int pressure;
} BMP280Data;

/**
 * @brief Configuration params for barometer
 */
typedef struct BMP280CalibParams
{
    uint16_t dig_t1;
    int16_t dig_t2;
    int16_t dig_t3;
    
    uint16_t dig_p1;
    int16_t dig_p2;
    int16_t dig_p3;
    int16_t dig_p4;
    int16_t dig_p5;
    int16_t dig_p6;
    int16_t dig_p7;
    int16_t dig_p8;
    int16_t dig_p9;
} BMP280CalibParams;

/**
 * @brief Barometer config data. All changes are done internally, so keep it just for reference!
 */
typedef struct BMP280Config
{
    int _i2c;
    BMP280CalibParams _calibParams;
} BMP280Config;

/**
 * @brief Initialize the barometer
 *
 * @param config Barometer config data
 * @param i2c I2C (0 or 1)
 * @param sda SDA pin
 * @param scl SCL pin
 * @return Result code.
 */
FUNCRESULT bmp280Init(BMP280Config *config, int i2c, int sda, int scl);

/**
 * @brief Checks if the barometer is initialized
 *
 * @param config Barometer config data
 * @param available Is Initialized
 * @return Result code.
 */
FUNCRESULT bmp280Check(BMP280Config *config, bool *result);

/**
 * @brief Read barometer data
 *
 * @param config Barometer config data
 * @param data Returns barometer data
 * @return Result code.
 */
FUNCRESULT bmp280Read(BMP280Config *config, BMP280Data *data);