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
 * @brief LSM6DSO32 Accelerometer and Gyroscope ODR
 */
typedef enum LSM6DSO32ODR
{
    LSM6DSO32_ODR_12_5HZ = 0x01,
    LSM6DSO32_ODR_26HZ = 0x02,
    LSM6DSO32_ODR_52HZ = 0x03,
    LSM6DSO32_ODR_104HZ = 0x04,
    LSM6DSO32_ODR_208HZ = 0x05,
    LSM6DSO32_ODR_416HZ = 0x06,
    LSM6DSO32_ODR_833HZ = 0x07,
    LSM6DSO32_ODR_1660HZ = 0x08,
    LSM6DSO32_ODR_3330HZ = 0x09,
    LSM6DSO32_ODR_6660HZ = 0x0A,
} LSM6DSO32ODR;

/**
 * @brief LSM6DSO32 Accelerometer range
 */
typedef enum LSM6DSO32AccelRange
{
    LSM6DSO32_RANGE_4G = 0x00,
    LSM6DSO32_RANGE_8G = 0x02,
    LSM6DSO32_RANGE_16G = 0x03,
    LSM6DSO32_RANGE_32G = 0x01,
} LSM6DSO32AccelRange;

/**
 * @brief LSM6DSO32 Gyroscope range
 */
typedef enum LSM6DSO32GyroRange
{
    LSM6DSO32_RANGE_125DPS = 0x04,
    LSM6DSO32_RANGE_250DPS = 0x00,
    LSM6DSO32_RANGE_500DPS = 0x01,
    LSM6DSO32_RANGE_1000DPS = 0x02,
    LSM6DSO32_RANGE_2000DPS = 0x03,
} LSM6DSO32GyroRange;

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
 * @brief Validate LSM6DSO32 ID
 *
 * @param config LSM6DSO configuration
 * @param valid Validity of ID
 * @return Result code
 */
FUNCRESULT lsm6dso32ValidateId(LSM6DSO32Config *config, BOOL *valid);

/**
 * @brief Set LSM6DSO32 ODR
 *
 * @param config LSM6DSO configuration
 * @param accelODR Accelerometer ODR
 * @param gyroODR Gyroscope ODR
 * @return Result code
 */
FUNCRESULT lsm6dso32SetODR(LSM6DSO32Config *config, LSM6DSO32ODR accelODR, LSM6DSO32ODR gyroODR);

/**
 * @brief Set LSM6DSO32 range
 *
 * @param config LSM6DSO configuration
 * @param accelRange Accelerometer range
 * @param gyroRange Gyroscope range
 * @return Result code
 */
FUNCRESULT lsm6dso32SetRange(LSM6DSO32Config *config, LSM6DSO32AccelRange accelRange, LSM6DSO32GyroRange gyroRange);

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