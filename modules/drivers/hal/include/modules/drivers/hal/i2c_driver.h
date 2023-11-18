#ifndef _I2C_DRIVER_H
#define _I2C_DRIVER_H

#include "gpio_driver.h"
#include "time_tracker.h"
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

typedef uint8_t i2c_instance_t; /** I2C instance */

#define I2C_INSTANCE_INVALID 0xFF /** I2C invalid instance */

/**
 * @brief Checks if I2C is valid
 *
 * @param i2c I2C Instance
 * @return True if instance is valid
 */
bool i2c_check_instance(i2c_instance_t i2c);

/**
 * @brief Checks if I2C SDA pin is valid
 *
 * @param i2c I2C instance
 * @param sda SDA pin
 * @return True if sda is valid
 */
bool i2c_check_sda(i2c_instance_t i2c, pin_number_t sda);

/**
 * @brief Checks if I2C SCL pin is valid
 *
 * @param i2c I2C instance
 * @param scl SCL pin
 * @return True if scl is valid
 */
bool i2c_check_scl(i2c_instance_t i2c, pin_number_t scl);

/**
 * @brief Initialize I2C instance
 *
 * @param i2c I2C Instance
 * @param baudrate Baud rate
 */
void i2c_init_all(i2c_instance_t i2c, unsigned long baudrate);

/**
 * @brief Initialize I2C pins
 *
 * @param i2c I2C Instance
 * @param sda SDA pin
 * @param scl SCL pin
 */
void i2c_init_pins(i2c_instance_t i2c, pin_number_t sda, pin_number_t scl);

/**
 * @brief Write to I2C with blocking
 *
 * @param i2c I2C Instance
 * @param address Address of I2C
 * @param data Data pointer to write
 * @param size Size of data
 * @param nostop No stop
 * @return True if operation was successful
 */
bool i2c_write(i2c_instance_t i2c, uint8_t address, const uint8_t *data, size_t size, bool nostop);

/**
 * @brief Read from I2C with blocking
 *
 * @param i2c I2C Instance
 * @param address Address of I2C
 * @param destination Data pointer to read to
 * @param size Size of data to receive
 * @param nostop No stop
 * @return True if operation was successful
 */
bool i2c_read(i2c_instance_t i2c, uint8_t address, uint8_t *destination, size_t size, bool nostop);

#endif