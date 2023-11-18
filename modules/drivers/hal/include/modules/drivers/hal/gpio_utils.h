#ifndef _GPIO_UTILS_H
#define _GPIO_UTILS_H

#include "gpio_driver.h"
#include "spi_driver.h"
#include "i2c_driver.h"
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

/**
 * @brief The basic configuration for a GPIO device. Set only necessary fields. 'protocol' field is required.
 */
typedef struct gpio_communication_config
{
    gpio_protocol_t protocol; /** Protocol to use */
    spi_instance_t spi;       /** SPI */
    pin_number_t cs;          /** CS */
    i2c_instance_t i2c;       /** I2C */
    uint8_t i2cAddress;       /** I2C address */
    uint8_t readMask;         /** Mask to apply to read values */
    uint8_t multipleReadMask; /** Mask to apply to multiple read values */
    uint8_t writeMask;        /** Mask to apply to write values */
} gpio_communication_config_t;

/**
 * @brief Writes register field
 *
 * @param config Device configuration
 * @param address Register address
 * @param length Field length
 * @param offset Field offset
 * @param value Field value
 * @return True if success
 */
bool gpio_write_reg_field(gpio_communication_config_t *config, uint8_t address, uint8_t length, uint8_t offset, uint8_t value);

/**
 * @brief Reads single register
 *
 * @param config Device configuration
 * @param address Register address
 * @return Register value
 */
uint8_t gpio_read_reg(gpio_communication_config_t *config, uint8_t address);

/**
 * @brief Reads multiple registers
 *
 * @param config Device configuration
 * @param address Starting register address
 * @param buffer Buffer to store data
 * @param count Count of registers to read
 */
void gpio_read_regs(gpio_communication_config_t *config, uint8_t address, uint8_t *buffer, size_t count);

/**
 * @brief Writes single register
 *
 * @param config Device configuration
 * @param address Register address
 * @param data Data to write
 */
void gpio_write_reg(gpio_communication_config_t *config, uint8_t address, uint8_t value);

/**
 * @brief Performs a single read operation
 *
 * @param config Device configuration
 * @return Read value
 */
uint8_t gpio_single_read(gpio_communication_config_t *config);

#endif