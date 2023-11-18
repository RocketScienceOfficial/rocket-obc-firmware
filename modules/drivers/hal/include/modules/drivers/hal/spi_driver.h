#ifndef _SPI_DRIVER_H
#define _SPI_DRIVER_H

#include "gpio_driver.h"
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

typedef uint8_t spi_instance_t; /** SPI instance */

#define SPI_INSTANCE_INVALID 0xFF /** SPI invalid instance */

/**
 * @brief Checks if SPI is valid
 *
 * @param instance SPI Instance
 * @return True if instance is valid
 */
bool spi_check_instance(spi_instance_t spi);

/**
 * @brief Checks if SPI MISO pin is valid
 *
 * @param spi SPI instance
 * @param miso MISO pin
 * @return True if miso is valid
 */
bool spi_check_miso(spi_instance_t spi, pin_number_t miso);

/**
 * @brief Checks if SPI MOSI pin is valid
 *
 * @param spi SPI instance
 * @param mosi MOSI pin
 * @return True if mosi is valid
 */
bool spi_check_mosi(spi_instance_t spi, pin_number_t mosi);

/**
 * @brief Checks if SPI SCK pin is valid
 *
 * @param spi SPI instance
 * @param sck SCK pin
 * @return True if sck is valid
 */
bool spi_check_sck(spi_instance_t spi, pin_number_t sck);

/**
 * @brief Initialize SPI instance
 *
 * @param spi SPI Instance
 * @param baudrate Baud rate
 */
void spi_init_all(spi_instance_t spi, unsigned long baudrate);

/**
 * @brief Initialize SPI pins
 *
 * @param spi SPI Instance
 * @param miso MISO pin
 * @param mosi MOSI pin
 * @param sck SCK pin
 * @param cs CS pin
 */
void spi_init_pins(spi_instance_t spi, pin_number_t miso, pin_number_t mosi, pin_number_t sck, pin_number_t cs);

/**
 * @brief Write to SPI with blocking
 *
 * @param spi SPI Instance
 * @param data Data pointer to write
 * @param size Size of data
 * @return True if operation was successful
 */
bool spi_write(spi_instance_t spi, const uint8_t *data, size_t size);

/**
 * @brief Read from SPI with blocking
 *
 * @param spi SPI Instance
 * @param repeatedTXData Buffer to data to write
 * @param destination Data pointer to read from
 * @param size Size of data to receive
 * @return True if operation was successful
 */
bool spi_read(spi_instance_t spi, uint8_t repeatedTXData, uint8_t *destination, size_t size);

/**
 * @brief Write and read from SPI with blocking
 *
 * @param spi SPI Instance
 * @param data Data pointer to write
 * @param destination Data pointer to read from
 * @param size Size of data
 * @return True if operation was successful
 */
bool spi_write_read(spi_instance_t spi, const uint8_t *data, uint8_t *destination, size_t size);

#endif