#pragma once

#include <obc/api.h>
#include "drivers/gpio/spi_driver.h"
#include "drivers/gpio/i2c_driver.h"
#include "drivers/gpio/gpio_driver.h"

/**
 * @brief BMP581 configuration
 */
typedef struct BMP581Config
{
    BOOL useSPI;     /** Use SPI */
    SPIInstance spi; /** SPI instance */
    PinNumber cs;    /** CS pin */
    I2CInstance i2c; /** I2C instance */
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
 * @brief Writes register field of BMP581
 *
 * @param config BMP581 configuration
 * @param address Register address
 * @param length Field length
 * @param offset Field offset
 * @param value Field value
 * @return Success or fail
 */
BOOL __bmp581WriteRegField(BMP581Config *config, BYTE address, UINT8 length, UINT8 offset, BYTE value);

/**
 * @brief Reads single register
 *
 * @param config BMP581 configuration
 * @param address Register address
 * @return Register value
 */
BYTE __bmp581ReadReg(BMP581Config *config, BYTE address);

/**
 * @brief Reads multiple registers
 *
 * @param config BMP581 configuration
 * @param address Starting register address
 * @param buffer Buffer to store data
 * @param count Count of registers to read
 */
VOID __bmp581ReadRegs(BMP581Config *config, BYTE address, BYTE *buffer, SIZE count);

/**
 * @brief Writes single register
 *
 * @param config BMP581 configuration
 * @param address Register address
 * @param data Data to write
 */
VOID __bmp581WriteReg(BMP581Config *config, BYTE address, BYTE data);