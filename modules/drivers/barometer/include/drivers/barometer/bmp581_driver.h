#pragma once

#include <obc/api.h>
#include "drivers/gpio/gpio_utils.h"

/**
 * @brief BMP581 configuration
 */
typedef struct BMP581Config
{
    GPIOCommunicationConfig gpioConfig; /** GPIO communication configuration */
} BMP581Config;

/**
 * @brief BMP581 data
 */
typedef struct BMP581Data
{
    FLOAT pressure;    /** Pressure in Pa */
    FLOAT temperature; /** Temperature in C */
} BMP581Data;

/**
 * @brief BMP581 OSR
 */
typedef enum BMP581OSR
{
    BMP581_OSR_1 = 0x00,
    BMP581_OSR_2 = 0x01,
    BMP581_OSR_4 = 0x02,
    BMP581_OSR_8 = 0x03,
    BMP581_OSR_16 = 0x04,
    BMP581_OSR_32 = 0x05,
    BMP581_OSR_64 = 0x06,
    BMP581_OSR_128 = 0x07,
} BMP581OSR;

/**
 * @brief BMP581 ODR
 */
typedef enum BMP581ODR
{
    BMP581_ODR_240 = 0x00,
    BMP581_ODR_220 = 0x01,
    BMP581_ODR_200 = 0x02,
    BMP581_ODR_180 = 0x03,
    BMP581_ODR_160 = 0x04,
    BMP581_ODR_150 = 0x05,
    BMP581_ODR_140 = 0x06,
    BMP581_ODR_130 = 0x07,
    BMP581_ODR_120 = 0x08,
    BMP581_ODR_110 = 0x09,
    BMP581_ODR_100 = 0x0A,
    BMP581_ODR_90 = 0x0B,
    BMP581_ODR_80 = 0x0C,
    BMP581_ODR_70 = 0x0D,
    BMP581_ODR_60 = 0x0E,
    BMP581_ODR_50 = 0x0F,
    BMP581_ODR_45 = 0x10,
    BMP581_ODR_40 = 0x11,
    BMP581_ODR_35 = 0x12,
    BMP581_ODR_30 = 0x13,
    BMP581_ODR_25 = 0x14,
    BMP581_ODR_20 = 0x15,
    BMP581_ODR_15 = 0x16,
    BMP581_ODR_10 = 0x17,
    BMP581_ODR_5 = 0x18,
    BMP581_ODR_4 = 0x19,
    BMP581_ODR_3 = 0x1A,
    BMP581_ODR_2 = 0x1B,
    BMP581_ODR_1 = 0x1C,
    BMP581_ODR_0500 = 0x1D,
    BMP581_ODR_0250 = 0x1E,
    BMP581_ODR_0125 = 0x1F,
} BMP581ODR;

/**
 * @brief BMP581 mode
 */
typedef enum BMP581Mode
{
    BMP581_MODE_SLEEP = 0x00,
    BMP581_MODE_NORMAL = 0x01,
    BMP581_MODE_FORCED = 0x02,
    BMP581_MODE_CONTINUOUS = 0x03,
} BMP581Mode;

/**
 * @brief Initializes BMP581 using SPI
 *
 * @param config BMP581 configuration
 * @param spi SPI instance
 * @param miso MISO pin
 * @param mosi MOSI pin
 * @param cs CS pin
 * @param sck SCK pin
 * @return Result code
 */
FUNCRESULT bmp581InitSPI(BMP581Config *config, SPIInstance spi, PinNumber miso, PinNumber mosi, PinNumber cs, PinNumber sck);

/**
 * @brief Initializes BMP581 using I2C
 *
 * @param config BMP581 configuration
 * @param i2c I2C instance
 * @param sda SDA pin
 * @param scl SCL pin
 * @return Result code
 */
FUNCRESULT bmp581InitI2C(BMP581Config *config, I2CInstance i2c, PinNumber sda, PinNumber scl);

/**
 * @brief Validates BMP581 ID
 *
 * @param config BMP581 configuration
 * @param pValid Pointer to store validation result
 * @return Result code
 */
FUNCRESULT bmp581ValidateId(BMP581Config *config, BOOL *pValid);

/**
 * @brief Sets BMP581 OSR of temperature and pressure
 *
 * @param config BMP581 configuration
 * @param tempOSR OSR of temperature
 * @param pressOSR OSR of pressure
 * @param odr ODR of BMP581. Refer to datasheet for allowed values!
 * @return Result code
 */
FUNCRESULT bmp581SetODR_OSR(BMP581Config *config, BMP581OSR tempOSR, BMP581OSR pressOSR, BMP581ODR odr);

/**
 * @brief Sets BMP581 mode
 *
 * @param config BMP581 configuration
 * @param mode BMP581 mode
 * @return Result code
 */
FUNCRESULT bmp581SetMode(BMP581Config *config, BMP581Mode mode);

/**
 * @brief Read BMP581 sensor data
 *
 * @param config BMP581 configuration
 * @param data BMP581 data pointer
 * @return Result code
 */
FUNCRESULT bmp581Read(BMP581Config *config, BMP581Data *data);

/**
 * @brief BMP581 soft reset
 *
 * @param config BMP581 configuration
 */
VOID __bmp581SoftReset(BMP581Config *config);