#pragma once

#include "tools/typedefs.h"
#include "drivers/gpio/gpio_typedefs.h"
#include "drivers/gpio/i2c_driver.h"

/**
 * @brief Barometer data
 */
typedef struct BMP280Data
{
    FLOAT temperature;
    INT32 pressure;
} BMP280Data;

/**
 * @brief Configuration params for barometer
 */
typedef struct BMP280CalibParams
{
    UINT16 dig_t1;
    INT16 dig_t2;
    INT16 dig_t3;
    
    UINT16 dig_p1;
    INT16 dig_p2;
    INT16 dig_p3;
    INT16 dig_p4;
    INT16 dig_p5;
    INT16 dig_p6;
    INT16 dig_p7;
    INT16 dig_p8;
    INT16 dig_p9;
} BMP280CalibParams;

/**
 * @brief Barometer config data. All changes are done internally, so keep it just for reference!
 */
typedef struct BMP280Config
{
    I2CInstance _i2c;
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
FUNCRESULT bmp280Init(BMP280Config *config, I2CInstance i2c, PinNumber sda, PinNumber scl);

/**
 * @brief Checks if the barometer is initialized
 *
 * @param config Barometer config data
 * @param available Is Initialized
 * @return Result code.
 */
FUNCRESULT bmp280Check(BMP280Config *config, BOOL *result);

/**
 * @brief Read barometer data
 *
 * @param config Barometer config data
 * @param data Returns barometer data
 * @return Result code.
 */
FUNCRESULT bmp280Read(BMP280Config *config, BMP280Data *data);