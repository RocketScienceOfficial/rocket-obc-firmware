#pragma once

#include "gpio_driver.h"
#include "tools/typedefs.h"
#include "tools/time_tracker.h"

/**
 * @brief I2C instance.
 */
typedef UINT8 I2CInstance;

/**
 * @brief I2C baud rate.
 */
typedef UINT32 I2CBaudRate;

/**
 * @brief I2C invalid instance.
 */
#define I2C_INSTANCE_INVALID 0xFF

/**
 * @brief Checks if I2C is valid.
 *
 * @param i2c I2C Instance.
 * @return True if instance is valid.
 */
BOOL i2cCheckInstance(I2CInstance i2c);

/**
 * @brief Checks if I2C SDA pin is valid.
 *
 * @param i2c I2C instance.
 * @param sda SDA pin.
 * @return True if sda is valid.
 */
BOOL i2cCheckSDA(I2CInstance i2c, PinNumber sda);

/**
 * @brief Checks if I2C SCL pin is valid.
 *
 * @param i2c I2C instance.
 * @param scl SCL pin.
 * @return True if scl is valid.
 */
BOOL i2cCheckSCL(I2CInstance i2c, PinNumber scl);

/**
 * @brief Initialize I2C instance.
 *
 * @param i2c I2C Instance.
 * @param baudrate Baud rate.
 * @return Result code.
 */
FUNCRESULT i2cInitAll(I2CInstance i2c, I2CBaudRate baudrate);

/**
 * @brief Initialize I2C pins.
 *
 * @param i2c I2C Instance.
 * @param sda SDA pin.
 * @param scl SCL pin.
 * @return Result code.
 */
FUNCRESULT i2cInitPins(I2CInstance i2c, PinNumber sda, PinNumber scl);

/**
 * @brief Write to I2C with timeout.
 *
 * @param i2c I2C Instance.
 * @param address Address of I2C.
 * @param data Data pointer to write.
 * @param size Size of data.
 * @param timeout Timeout in us.
 * @param nostop No stop.
 * @return Result code.
 */
FUNCRESULT i2cWriteTimeout(I2CInstance i2c, BYTE address, BYTE *data, SIZE size, TIME timeout, BOOL nostop);

/**
 * @brief Read from I2C with timeout.
 *
 * @param i2c I2C Instance.
 * @param address Address of I2C.
 * @param destination Data pointer to read to.
 * @param size Size of data to receive.
 * @param timeout Timeout in us.
 * @param nostop No stop.
 * @return Result code.
 */
FUNCRESULT i2cReadTimeout(I2CInstance i2c, BYTE address, BYTE *destination, SIZE size, TIME timeout, BOOL nostop);

/**
 * @brief Write to I2C with blocking.
 *
 * @param i2c I2C Instance.
 * @param address Address of I2C.
 * @param data Data pointer to write.
 * @param size Size of data.
 * @param nostop No stop.
 * @return Result code.
 */
FUNCRESULT i2cWriteBlocking(I2CInstance i2c, BYTE address, const BYTE *data, SIZE size, BOOL nostop);

/**
 * @brief Read from I2C with blocking.
 *
 * @param i2c I2C Instance.
 * @param address Address of I2C.
 * @param destination Data pointer to read to.
 * @param size Size of data to receive.
 * @param nostop No stop.
 * @return Result code.
 */
FUNCRESULT i2cReadBlocking(I2CInstance i2c, BYTE address, BYTE *destination, SIZE size, BOOL nostop);

/**
 * @brief Check if I2C device is valid.
 * 
 * @param i2c I2C Instance to check.
 * @param address Address of device.
 * @return True if device is valid
 */
BOOL i2cCheckDevice(I2CInstance i2c, BYTE address);