#pragma once

#include <obc/api.h>
#include "drivers/gpio/gpio_utils.h"
#include "maths/vector.h"

/**
 * @brief LSM6DSO Accelerometer configuration
 */
typedef struct LSM6DSO32Config
{
    GPIOCommunicationConfig gpioConfig; /** GPIO communication config */
    FLOAT accelRangeConstant;           /** Accelerometer range constant */
    FLOAT gyroRangeConstant;            /** Gyroscope range constant */
} LSM6DSO32Config;

/**
 * @brief Initialize LSM6DSO accelerometer using SPI
 *
 * @param config LSM6DSO configuration
 * @param spi SPI instance
 * @param miso MISO pin
 * @param mosi MOSI pin
 * @param cs CS pin
 * @param sck SCK pin
 * @return Result code
 */
FUNCRESULT lsm6dso32InitSPI(LSM6DSO32Config *config, SPIInstance spi, PinNumber miso, PinNumber mosi, PinNumber cs, PinNumber sck);

/**
 * @brief Initialize LSM6DSO accelerometer using I2C
 *
 * @param config LSM6DSO configuration
 * @param i2c I2C instance
 * @param sda SDA pin
 * @param scl SCL pin
 * @return Result code
 */
FUNCRESULT lsm6dso32InitI2C(LSM6DSO32Config *config, I2CInstance i2c, PinNumber sda, PinNumber scl);

/**
 * @brief Read LSM6DSO acceleration and angular rate
 *
 * @param config LSM6DSO configuration
 * @param pAcceleration Acceleration in meters per second squared
 * @param pGyro Angular rate in radians per second
 * @param pTemperature Temperature in degrees Celsius
 * @return Result code
 */
FUNCRESULT lsm6dso32Read(LSM6DSO32Config *config, vec3 *pAcceleration, vec3 *pGyro, FLOAT *pTemperature);