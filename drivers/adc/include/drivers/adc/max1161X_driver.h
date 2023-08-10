#pragma once

#include <obc/api.h>
#include "drivers/gpio/i2c_driver.h"

/**
 * @brief MAX1161X configuration
 */
typedef struct MAX1161XConfig
{
    I2CInstance i2c;
    PinNumber sda;
    PinNumber scl;
    BYTE address;
    FLOAT vRef;
} MAX1161XConfig;

/**
 * @brief MAX1161X type
 */
typedef enum MAX1161XType
{
    MAX1161X_TYPE_2,
    MAX1161X_TYPE_3,
    MAX1161X_TYPE_4,
    MAX1161X_TYPE_5,
    MAX1161X_TYPE_6,
    MAX1161X_TYPE_7,
} MAX1161XType;

/**
 * @brief MAX1161X channel
 */
typedef enum MAX1161XChannel
{
    MAX1161X_CHANNEL_AIN0 = 0x00,
    MAX1161X_CHANNEL_AIN1 = 0x01,
    MAX1161X_CHANNEL_AIN2 = 0x02,
    MAX1161X_CHANNEL_AIN3 = 0x03,
    MAX1161X_CHANNEL_AIN4 = 0x04,
    MAX1161X_CHANNEL_AIN5 = 0x05,
    MAX1161X_CHANNEL_AIN6 = 0x06,
    MAX1161X_CHANNEL_AIN7 = 0x07,
    MAX1161X_CHANNEL_AIN8 = 0x08,
    MAX1161X_CHANNEL_AIN9 = 0x09,
    MAX1161X_CHANNEL_AIN10 = 0x0A,
    MAX1161X_CHANNEL_AIN11 = 0x0B,
} MAX1161XChannel;

/**
 * @brief Initializes MAX1161X
 *
 * @param config Configuration
 * @param type Type
 * @param i2c I2C instance
 * @param sda SDA pin
 * @param scl SCL pin
 * @return Result code
 */
FUNCRESULT max1161XInit(MAX1161XConfig *config, MAX1161XType type, I2CInstance i2c, PinNumber sda, PinNumber scl);

/**
 * @brief Reads MAX1161X value
 *
 * @param config Configuration
 * @param channel Channel
 * @param voltage Voltage value to return
 * @return Result code
 */
FUNCRESULT max1161XRead(MAX1161XConfig *config, MAX1161XChannel channel, VoltageLevel *voltage);

/**
 * @brief Sets up MAX1161X
 *
 * @param config Configuration
 * @param data Data
 */
VOID __max1161XSetup(MAX1161XConfig *config, BYTE data);

/**
 * @brief Configures MAX1161X
 *
 * @param config Configuration
 * @param data Data
 */
VOID __max1161XConfig(MAX1161XConfig *config, BYTE data);