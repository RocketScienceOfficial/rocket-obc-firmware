#ifndef _SPI_DRIVER_H
#define _SPI_DRIVER_H

#include "gpio_driver.h"
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

typedef uint8_t hal_spi_instance_t; /** SPI instance */

/**
 * @brief Initialize SPI
 *
 * @param spi SPI Instance
 * @param miso MISO pin
 * @param mosi MOSI pin
 * @param sck SCK pin
 * @param baudrate Baud rate
 * @return true if success
 */
bool hal_spi_init_all(hal_spi_instance_t spi, hal_pin_number_t miso, hal_pin_number_t mosi, hal_pin_number_t sck, hal_baud_rate_t baudrate);

/**
 * @brief Initializes CS pin
 *
 * @param spi SPI Instance
 * @param cs CS pin
 * @return true if success
 */
bool hal_spi_init_cs(hal_pin_number_t cs);

/**
 * @brief Selects CS pin of SPI
 *
 * @param cs CS Pin
 * @return true if success
 */
bool hal_spi_cs_select(hal_pin_number_t cs);

/**
 * @brief Deselects CS pin of SPI
 *
 * @param cs CS Pin
 * @return true if success
 */
bool hal_spi_cs_deselect(hal_pin_number_t cs);

/**
 * @brief Write to SPI with blocking
 *
 * @param spi SPI Instance
 * @param data Data pointer to write
 * @param size Size of data
 * @return True if operation was successful
 */
bool hal_spi_write(hal_spi_instance_t spi, const uint8_t *data, size_t size);

/**
 * @brief Read from SPI with blocking
 *
 * @param spi SPI Instance
 * @param repeatedTXData Buffer to data to write
 * @param destination Data pointer to read from
 * @param size Size of data to receive
 * @return True if operation was successful
 */
bool hal_spi_read(hal_spi_instance_t spi, uint8_t repeatedTXData, uint8_t *destination, size_t size);

/**
 * @brief Write and read from SPI with blocking
 *
 * @param spi SPI Instance
 * @param data Data pointer to write
 * @param destination Data pointer to read from
 * @param size Size of data
 * @return True if operation was successful
 */
bool hal_spi_write_read(hal_spi_instance_t spi, const uint8_t *data, uint8_t *destination, size_t size);

#endif