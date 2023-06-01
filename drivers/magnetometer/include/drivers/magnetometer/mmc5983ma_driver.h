#pragma once

/**
 * REF: https://github.com/kriswiner/MMC5983MA/tree/master
 */

#include <obc/api.h>
#include "drivers/gpio/spi_driver.h"
#include "drivers/gpio/gpio_driver.h"
#include "maths/vector.h"

/**
 * @brief MMC5983MA Magnetometer configuration
 */
typedef struct MMC5983MAConfig
{
    SPIInstance spi; /** SPI */
    PinNumber cs;    /** CS */
} MMC5983MAConfig;

/**
 * @brief MMC5983MA Magnetometer bandwidth
 */
typedef enum MMC5983MABandwidth
{
    MMC5983MA_BW_100HZ = 0x00,
    MMC5983MA_BW_200HZ = 0x01,
    MMC5983MA_BW_400HZ = 0x02,
    MMC5983MA_BW_800HZ = 0x03,
} MMC5983MABandwidth;

/**
 * @brief MMC5983MA Magnetometer ODR
 */
typedef enum MMC5983ODR
{
    MMC5983MA_ODR_1HZ = 0x01,
    MMC5983MA_ODR_10HZ = 0x02,
    MMC5983MA_ODR_20HZ = 0x03,
    MMC5983MA_ODR_50HZ = 0x04,
    MMC5983MA_ODR_100HZ = 0x05,
    MMC5983MA_ODR_200HZ = 0x06,
    MMC5983MA_ODR_1000HZ = 0x07,
} MMC5983ODR;

/**
 * @brief MMC5983MA Magnetometer initialization
 *
 * @param config MMC5983MA configuration
 * @param spi SPI instance
 * @param miso MISO pin
 * @param mosi MOSI pin
 * @param cs CS pin
 * @param sck SCK pin
 * @return Result code
 */
FUNCRESULT mmc5983maInit(MMC5983MAConfig *config, SPIInstance spi, PinNumber miso, PinNumber mosi, PinNumber cs, PinNumber sck);

/**
 * @brief Check if product id is valid
 *
 * @param config MMC5983MA configuration
 * @param valid Validity
 * @return Result code
 */
FUNCRESULT mmc5983ValidateId(MMC5983MAConfig *config, BOOL *valid);

/**
 * @brief Set bandwidth of magnetometer
 *
 * @param config MMC5983MA configuration
 * @param bandwidth Bandwidth
 * @return Result code
 */
FUNCRESULT mmc5983maSetBandwidth(MMC5983MAConfig *config, MMC5983MABandwidth bandwidth);

/**
 * @brief Set ODR of magnetometer
 *
 * @param config MMC5983MA configuration
 * @param odr ODR
 * @return Result code
 */
FUNCRESULT mmc5983maSetODR(MMC5983MAConfig *config, MMC5983ODR odr);

/**
 * @brief MMC5983MA Magnetometer reset
 *
 * @param config MMC5983MA configuration
 * @return Result code
 */
FUNCRESULT mmc5983maReset(MMC5983MAConfig *config);

/**
 * @brief Calculate offsets of magnetometer
 *
 * @param config MMC5983MA configuration
 * @param offset Offset vector
 * @return Result code
 */
FUNCRESULT mmc5983maCalculateOffset(MMC5983MAConfig *config, vec3 *offset);

/**
 * @brief Read data from magnetometer
 *
 * @param config MMC5983MA configuration
 * @param mag Magnetic field in Gauss
 * @return Result code
 */
FUNCRESULT mmc5983maRead(MMC5983MAConfig *config, vec3 *mag);

/**
 * @brief Read temperature from magnetometer
 *
 * @param config MMC5983MA configuration
 * @param temp Temperature
 * @return Result code
 */
FUNCRESULT mmc5983maReadTemp(MMC5983MAConfig *config, FLOAT *temp);

/**
 * @brief Enable SET current
 *
 * @param config MMC5983MA configuration
 */
VOID __mmc5983maSET(MMC5983MAConfig *config);

/**
 * @brief Enable RESET current
 *
 * @param config MMC5983MA configuration
 */
VOID __mmc5983maRESET(MMC5983MAConfig *config);

/**
 * @brief Read register from magnetometer
 *
 * @param config MMC5983MA configuration
 * @param address Register address
 * @return Register value
 */
BYTE __mmc5983maReadReg(MMC5983MAConfig *config, BYTE address);

/**
 * @brief Read registers from magnetometer
 *
 * @param config MMC5983MA configuration
 * @param address Register address
 * @param buffer Buffer
 * @param count Count
 */
VOID __mmc5983maReadRegs(MMC5983MAConfig *config, BYTE address, BYTE *buffer, SIZE count);

/**
 * @brief Write register to magnetometer
 *
 * @param config MMC5983MA configuration
 * @param address Register address
 * @param data Register value
 */
VOID __mmc5983maWriteReg(MMC5983MAConfig *config, BYTE address, BYTE data);