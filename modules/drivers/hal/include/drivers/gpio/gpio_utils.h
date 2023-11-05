#pragma once

#include <obc/api.h>
#include "drivers/gpio/gpio_driver.h"
#include "drivers/gpio/spi_driver.h"
#include "drivers/gpio/i2c_driver.h"

/**
 * @brief The basic configuration for a GPIO device. Set only necessary fields. 'protocol' field is required.
 */
typedef struct GPIOCommunicationConfig
{
    GPIOProtocol protocol; /** Protocol to use */
    SPIInstance spi;       /** SPI */
    PinNumber cs;          /** CS */
    I2CInstance i2c;       /** I2C */
    BYTE i2cAddress;       /** I2C address */
    BYTE readMask;         /** Mask to apply to read values */
    BYTE multipleReadMask; /** Mask to apply to multiple read values */
    BYTE writeMask;        /** Mask to apply to write values */
} GPIOCommunicationConfig;

/**
 * @brief Writes register field
 *
 * @param config Device configuration
 * @param address Register address
 * @param length Field length
 * @param offset Field offset
 * @param value Field value
 * @return Success or fail
 */
BOOL gpioWriteRegField(GPIOCommunicationConfig *config, BYTE address, UINT8 length, UINT8 offset, BYTE value);

/**
 * @brief Reads single register
 *
 * @param config Device configuration
 * @param address Register address
 * @return Register value
 */
BYTE gpioReadReg(GPIOCommunicationConfig *config, BYTE address);

/**
 * @brief Reads multiple registers
 *
 * @param config Device configuration
 * @param address Starting register address
 * @param buffer Buffer to store data
 * @param count Count of registers to read
 */
VOID gpioReadRegs(GPIOCommunicationConfig *config, BYTE address, BYTE *buffer, SIZE count);

/**
 * @brief Writes single register
 *
 * @param config Device configuration
 * @param address Register address
 * @param data Data to write
 */
VOID gpioWriteReg(GPIOCommunicationConfig *config, BYTE address, BYTE value);

/**
 * @brief Performs a single read operation
 *
 * @param config Device configuration
 * @return Read value
 */
BYTE gpioSingleRead(GPIOCommunicationConfig *config);