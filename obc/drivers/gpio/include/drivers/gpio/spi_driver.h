#pragma once

#include "gpio_driver.h"
#include <obc/api.h>

/**
 * @brief I2C instance.
 */
typedef UINT8 SPIInstance;

/**
 * @brief I2C baud rate.
 */
typedef UINT32 SPIBaudRate;

/**
 * @brief I2C invalid instance.
 */
#define SPI_INSTANCE_INVALID 0xFF

/**
 * @brief Checks if I2C is valid.
 *
 * @param instance I2C Instance.
 * @return True if instance is valid.
 */
BOOL spiCheckInstance(SPIInstance spi);

/**
 * @brief Checks if SPI MISO pin is valid.
 *
 * @param spi SPI instance.
 * @param miso MISO pin.
 * @return True if miso is valid.
 */
BOOL spiCheckMISO(SPIInstance spi, PinNumber miso);

/**
 * @brief Checks if SPI MOSI pin is valid.
 *
 * @param spi SPI instance.
 * @param mosi MOSI pin.
 * @return True if mosi is valid.
 */
BOOL spiCheckMOSI(SPIInstance spi, PinNumber mosi);

/**
 * @brief Checks if SPI SCK pin is valid.
 *
 * @param spi SPI instance.
 * @param sck SCK pin.
 * @return True if sck is valid.
 */
BOOL spiCheckSCK(SPIInstance spi, PinNumber sck);

/**
 * @brief Checks if SPI CS pin is valid.
 *
 * @param spi SPI instance.
 * @param cs CS pin.
 * @return True if cs is valid.
 */
BOOL spiCheckCS(SPIInstance spi, PinNumber cs);

/**
 * @brief Initialize SPI instance.
 *
 * @param spi SPI Instance.
 * @param baudrate Baud rate.
 * @return Result code.
 */
FUNCRESULT spiInitAll(SPIInstance spi, SPIBaudRate baudrate);

/**
 * @brief Initialize SPI pins.
 *
 * @param spi SPI Instance.
 * @param miso MISO pin.
 * @param mosi MOSI pin.
 * @param sck SCK pin.
 * @param cs CS pin.
 * @return Result code.
 */
FUNCRESULT spiInitPins(SPIInstance spi, PinNumber miso, PinNumber mosi, PinNumber sck, PinNumber cs);

/**
 * @brief Write to SPI with blocking.
 *
 * @param spi SPI Instance.
 * @param data Data pointer to write.
 * @param size Size of data.
 * @return Result code.
 */
FUNCRESULT spiWriteBlocking(SPIInstance spi, const BYTE *data, SIZE size);

/**
 * @brief Read from SPI with blocking.
 *
 * @param spi SPI Instance.
 * @param repeatedTXData Buffer to data to write.
 * @param destination Data pointer to read from.
 * @param size Size of data to receive.
 * @return Result code.
 */
FUNCRESULT spiReadBlocking(SPIInstance spi, BYTE repeatedTXData, BYTE *destination, SIZE size);

/**
 * @brief Write and read from SPI with blocking.
 *
 * @param spi SPI Instance.
 * @param data Data pointer to write.
 * @param destination Data pointer to read from.
 * @param size Size of data.
 * @return Result code.
 */
FUNCRESULT spiWriteReadBlocking(SPIInstance spi, const BYTE *data, BYTE *destination, SIZE size);

/**
 * @brief Check if SPI device is valid.
 *
 * @param spi SPI Instance to check.
 * @return True if device is valid
 */
BOOL spiCheckDevice(SPIInstance spi);