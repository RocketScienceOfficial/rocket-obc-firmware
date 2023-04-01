#pragma once

#include <obc/api.h>
#include "drivers/gpio/spi_driver.h"
#include "drivers/gpio/gpio_driver.h"
#include "maths/vector.h"

/**
 * @brief H3lis331dl configuration
 */
typedef struct H3lis331dlConfig
{
    SPIInstance spi;   /** SPI */
    PinNumber cs;      /** CS */
    FLOAT rangeFactor; /** Range factor */
} H3lis331dlConfig;

/**
 * @brief H3lis331dl data
 */
typedef struct H3lis331dlData
{
    vec3 accel; /** Acceleration in meters per second squared */
} H3lis331dlData;

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
 * @brief H3lis331dl range
 */
typedef enum H3lis331dlRange
{
    H3LIS331DL_RANGE_100G = 0x00,
    H3LIS331DL_RANGE_200G = 0x01,
    H3LIS331DL_RANGE_400G = 0x03,
} H3lis331dlRange;

/**
 * @brief Initializes H3lis331dl
 *
 * @param config H3lis331dl configuration
 * @param spi SPI
 * @param miso MISO
 * @param mosi MOSI
 * @param cs CS
 * @param sck SCK
 * @return Result code
 */
FUNCRESULT h3lis331dlInit(H3lis331dlConfig *config, SPIInstance spi, PinNumber miso, PinNumber mosi, PinNumber cs, PinNumber sck);

/**
 * @brief Reads WHO_AM_I register
 *
 * @param config H3lis331dl configuration
 * @param whoami WHO_AM_I register value
 * @return Result code
 */
FUNCRESULT h3lis331dlWhoAmI(H3lis331dlConfig *config, BYTE *whoami);

/**
 * @brief Sets power
 *
 * @param config H3lis331dl configuration
 * @param power Power
 * @return Result code
 */
FUNCRESULT h3lis331dlSetPower(H3lis331dlConfig *config, BOOL power);

/**
 * @brief Sets output data rate
 *
 * @param config H3lis331dl configuration
 * @param odr Output data rate
 * @return Result code
 */
FUNCRESULT h3lis331dlSetODR(H3lis331dlConfig *config, H3lis331dlODR odr);

/**
 * @brief Sets range
 *
 * @param config H3lis331dl configuration
 * @param range Range
 * @return Result code
 */
FUNCRESULT h3lis331dlSetRange(H3lis331dlConfig *config, H3lis331dlRange range);

/**
 * @brief Reads status
 *
 * @param config H3lis331dl configuration
 * @param status Status
 * @return Result code
 */
FUNCRESULT h3lis33dlStatus(H3lis331dlConfig *config, BYTE *status);

/**
 * @brief Reads H3lis331dl
 *
 * @param config H3lis331dl configuration
 * @param data H3lis331dl data
 * @return Result code
 */
FUNCRESULT h3lis331dlRead(H3lis331dlConfig *config, H3lis331dlData *data);

/**
 * @brief Reads single register
 *
 * @param config H3lis331dl configuration
 * @param address Register address
 * @return Register value
 */
BYTE __h3lis331dlReadReg(H3lis331dlConfig *config, BYTE address);

/**
 * @brief Reads multiple registers
 *
 * @param config H3lis331dl configuration
 * @param address Starting register address
 * @param buffer Buffer to store data
 * @param count Count of registers to read
 */
VOID __h3lis331dlReadRegs(H3lis331dlConfig *config, BYTE address, BYTE *buffer, SIZE count);

/**
 * @brief Writes single register
 *
 * @param config H3lis331dl configuration
 * @param address Register address
 * @param data Data to write
 */
VOID __h3lis331dlWriteReg(H3lis331dlConfig *config, BYTE address, BYTE data);