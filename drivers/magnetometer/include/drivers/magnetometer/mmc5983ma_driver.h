#pragma once

/**
 * REF: https://github.com/kriswiner/MMC5983MA/tree/master
 * REF: https://www.vectornav.com/resources/inertial-navigation-primer/specifications--and--error-budgets/specs-hsicalibration
 */

#include <obc/api.h>
#include "drivers/gpio/gpio_utils.h"
#include "maths/vector.h"

/**
 * @brief MMC5983MA Magnetometer configuration
 */
typedef struct MMC5983MAConfig
{
    GPIOCommunicationConfig gpioConfig; /** The GPIO communication configuration */
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
 * @brief MMC5983MA Magnetometer initialization using SPI
 *
 * @param config MMC5983MA configuration
 * @param spi SPI instance
 * @param miso MISO pin
 * @param mosi MOSI pin
 * @param cs CS pin
 * @param sck SCK pin
 * @return Result code
 */
FUNCRESULT mmc5983maInitSPI(MMC5983MAConfig *config, SPIInstance spi, PinNumber miso, PinNumber mosi, PinNumber cs, PinNumber sck);

/**
 * @brief MMC5983MA Magnetometer initialization using I2C
 *
 * @param config MMC5983MA configuration
 * @param i2c I2C instance
 * @param sda SDA pin
 * @param scl SCL pin
 * @return Result code
 */
FUNCRESULT mmc5983maInitI2C(MMC5983MAConfig *config, I2CInstance i2c, PinNumber sda, PinNumber scl);

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
FUNCRESULT mmc5983maSetContinuousModeODR(MMC5983MAConfig *config, MMC5983ODR odr);

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
 * @param mag Magnetic field in miliGauss
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
 * @brief Magnetometer initialization base
 *
 * @param config MMC5983MA configuration
 */
VOID __mmc5983maInitBase(MMC5983MAConfig *config);