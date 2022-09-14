#pragma once

#include <stdbool.h>

/**
 * @brief Check if pin is valid.
 */
bool pinoutCheckPin(int pin);

/**
 * @brief Check if the given I2C bus is valid.
 * @param i2c The I2C bus to check.
 * @return True if the I2C bus is valid, false otherwise.
 */
bool pinoutCheckI2C(int i2c);

/**
 * @brief Check if the given I2C SDA pin is valid.
 * @param i2c The I2C bus to check.
 * @param sda The I2C SDA pin to check.
 * @return True if the I2C SDA pin is valid, false otherwise.
 */
bool pinoutCheckI2C_SDA(int i2c, int sda);

/**
 * @brief Check if the given I2C SCL pin is valid.
 * @param i2c The I2C bus to check.
 * @param scl The I2C SCL pin to check.
 * @return True if the I2C SCL pin is valid, false otherwise.
 */
bool pinoutCheckI2C_SCL(int i2c, int scl);