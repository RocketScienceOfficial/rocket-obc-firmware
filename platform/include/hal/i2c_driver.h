#ifndef _I2C_DRIVER_H
#define _I2C_DRIVER_H

#include "gpio_driver.h"
#include "time_tracker.h"
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

typedef uint8_t hal_i2c_instance_t; /** I2C instance */

#define I2C_INSTANCE_INVALID 0xFF /** I2C invalid instance */

/**
 * @brief Checks if I2C is valid
 *
 * @param i2c I2C Instance
 * @return True if instance is valid
 */
bool hal_i2c_check_instance(hal_i2c_instance_t i2c);

/**
 * @brief Checks if I2C SDA pin is valid
 *
 * @param i2c I2C instance
 * @param sda SDA pin
 * @return True if sda is valid
 */
bool hal_i2c_check_sda(hal_i2c_instance_t i2c, hal_pin_number_t sda);

/**
 * @brief Checks if I2C SCL pin is valid
 *
 * @param i2c I2C instance
 * @param scl SCL pin
 * @return True if scl is valid
 */
bool hal_i2c_check_scl(hal_i2c_instance_t i2c, hal_pin_number_t scl);

/**
 * @brief Initialize I2C
 *
 * @param i2c I2C Instance
 * @param sda SDA pin
 * @param scl SCL pin
 * @param baudrate Baud rate
 */
void hal_i2c_init_all(hal_i2c_instance_t i2c, hal_pin_number_t sda, hal_pin_number_t scl, hal_baud_rate_t baudrate);

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
bool hal_i2c_write(hal_i2c_instance_t i2c, uint8_t address, const uint8_t *data, size_t size, bool nostop);

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
bool hal_i2c_read(hal_i2c_instance_t i2c, uint8_t address, uint8_t *destination, size_t size, bool nostop);

#endif