#pragma once

#include <obc/api.h>
#include "drivers/gpio/gpio_utils.h"
#include "maths/vector.h"

#define H3LIS331DL_DEBUG TRUE

/**
 * @brief H3lis331dl configuration
 */
typedef struct H3lis331dlConfig
{
    GPIOCommunicationConfig gpioConfig; /** GPIO communication configuration */
    FLOAT rangeFactor;                  /** Range factor */
} H3lis331dlConfig;

/**
 * @brief H3lis331dl power mode
 */
typedef enum H3lis331dlPowerMode
{
    H3LIS331DL_POWER_DOWN = 0x00,
    H3LIS331DL_POWER_NORMAL = 0x01,
    H3LIS331DL_LOW_POWER_1 = 0x02,
    H3LIS331DL_LOW_POWER_2 = 0x03,
    H3LIS331DL_LOW_POWER_3 = 0x04,
    H3LIS331DL_LOW_POWER_4 = 0x05,
    H3LIS331DL_LOW_POWER_5 = 0x06,
} H3lis331dlPowerMode;

/**
 * @brief H3lis331dl output data rate
 */
typedef enum H3lis331dlODR
{
    H3LIS331DL_ODR_50HZ = 0x00,
    H3LIS331DL_ODR_100HZ = 0x01,
    H3LIS331DL_ODR_400HZ = 0x02,
    H3LIS331DL_ODR_1000HZ = 0x03,
} H3lis331dlODR;

/**
 * @brief H3lis331dl high pass filter cut off frequency coefficient
 */
typedef enum H3lis331dlHPFc
{
    H3LIS331DL_HPF_8 = 0x00,
    H3LIS331DL_HPF_16 = 0x01,
    H3LIS331DL_HPF_32 = 0x02,
    H3LIS331DL_HPF_64 = 0x03,
} H3lis331dlHPFc;

/**
 * @brief H3lis331dl range
 */
typedef enum H3lis331dlRange
{
    H3LIS331DL_RANGE_100G = 0x00,
    H3LIS331DL_RANGE_200G = 0x01,
    H3LIS331DL_RANGE_400G = 0x03,
} H3lis331dlRange;

/**
 * @brief Initializes H3lis331dl with SPI
 *
 * @param config H3lis331dl configuration
 * @param spi SPI
 * @param miso MISO
 * @param mosi MOSI
 * @param cs CS
 * @param sck SCK
 * @return Result code
 */
FUNCRESULT h3lis331dlInitSPI(H3lis331dlConfig *config, SPIInstance spi, PinNumber miso, PinNumber mosi, PinNumber cs, PinNumber sck);

/**
 * @brief Initializes H3lis331dl with I2C
 *
 * @param config H3lis331dl configuration
 * @param i2c I2C
 * @param sda SDA
 * @param scl SCL
 * @return Result code
 */
FUNCRESULT h3lis331dlInitI2C(H3lis331dlConfig *config, I2CInstance i2c, PinNumber sda, PinNumber scl);

/**
 * @brief Checks if H3lis331dl WHO_AM_I register is valid
 *
 * @param config H3lis331dl configuration
 * @param valid True if valid
 * @return Result code
 */
FUNCRESULT h3lis331dlValidateId(H3lis331dlConfig *config, BOOL *valid);

/**
 * @brief Sets power
 *
 * @param config H3lis331dl configuration
 * @param power Power
 * @return Result code
 */
FUNCRESULT h3lis331dlSetPowerMode(H3lis331dlConfig *config, H3lis331dlPowerMode power);

/**
 * @brief Sets output data rate
 *
 * @param config H3lis331dl configuration
 * @param odr Output data rate
 * @return Result code
 */
FUNCRESULT h3lis331dlSetODR(H3lis331dlConfig *config, H3lis331dlODR odr);

/**
 * @brief Sets high pass filter cut off frequency coefficient
 *
 * @param config H3lis331dl configuration
 * @param hpcf High pass filter cut off frequency coefficient
 * @return Result code
 */
FUNCRESULT h3lis331dlSetHPCF(H3lis331dlConfig *config, H3lis331dlHPFc hpcf);

/**
 * @brief Sets range
 *
 * @param config H3lis331dl configuration
 * @param range Range
 * @return Result code
 */
FUNCRESULT h3lis331dlSetRange(H3lis331dlConfig *config, H3lis331dlRange range);

/**
 * @brief Reads H3lis331dl
 *
 * @param config H3lis331dl configuration
 * @param accel H3lis331dl acceleration
 * @return Result code
 */
FUNCRESULT h3lis331dlRead(H3lis331dlConfig *config, vec3 *accel);