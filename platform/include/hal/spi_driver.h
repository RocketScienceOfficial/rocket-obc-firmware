#ifndef _SPI_DRIVER_H
#define _SPI_DRIVER_H

#include "gpio_driver.h"
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

typedef uint8_t hal_spi_instance_t; /** SPI instance */

#define SPI_INSTANCE_INVALID 0xFF /** SPI invalid instance */

/**
 * @brief Checks if SPI is valid
 *
 * @param instance SPI Instance
 * @return True if instance is valid
 */
bool hal_spi_check_instance(hal_spi_instance_t spi);

/**
 * @brief Checks if SPI MISO pin is valid
 *
 * @param spi SPI instance
 * @param miso MISO pin
 * @return True if miso is valid
 */
bool hal_spi_check_miso(hal_spi_instance_t spi, hal_pin_number_t miso);

/**
 * @brief Checks if SPI MOSI pin is valid
 *
 * @param spi SPI instance
 * @param mosi MOSI pin
 * @return True if mosi is valid
 */
bool hal_spi_check_mosi(hal_spi_instance_t spi, hal_pin_number_t mosi);

/**
 * @brief Checks if SPI SCK pin is valid
 *
 * @param spi SPI instance
 * @param sck SCK pin
 * @return True if sck is valid
 */
bool hal_spi_check_sck(hal_spi_instance_t spi, hal_pin_number_t sck);

/**
 * @brief Initialize SPI instance
 *
 * @param spi SPI Instance
 * @param baudrate Baud rate
 */
void hal_spi_init_all(hal_spi_instance_t spi, unsigned long baudrate);

/**
 * @brief Initialize SPI pins
 *
 * @param spi SPI Instance
 * @param miso MISO pin
 * @param mosi MOSI pin
 * @param sck SCK pin
 */
void hal_spi_init_pins(hal_spi_instance_t spi, hal_pin_number_t miso, hal_pin_number_t mosi, hal_pin_number_t sck);

/**
 * @brief Initializes CS pin
 * 
 * @param spi SPI Instance
 * @param cs CS pin
*/
void hal_spi_init_cs(hal_spi_instance_t spi, hal_pin_number_t cs);

/**
 * @brief Selects CS pin of SPI
 *
 * @param cs CS Pin
 */
void hal_spi_cs_select(hal_pin_number_t cs);

/**
 * @brief Deselects CS pin of SPI
 *
 * @param cs CS Pin
 */
void hal_spi_cs_deselect(hal_pin_number_t cs);

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