/**
 * Taken from:
 *
 * https://github.com/raspberrypi/pico-examples/tree/develop/i2c/lcd_1602_i2c
 */

#pragma once

#include <obc/api.h>
#include "drivers/gpio/i2c_driver.h"

/**
 * @brief LCD config data.
 */
typedef struct LCDConfig
{
    /**
     * @brief I2C bus. INTERNAL USE!
     */
    I2CInstance _i2c;
} LCDConfig;

/**
 * @brief Initialize the LCD display.
 *
 * @param config LCD config data.
 * @param i2c I2C bus.
 * @param sda SDA pin.
 * @param scl SCL pin.
 * @return Result code.
 */
FUNCRESULT lcdInit(LCDConfig *config, I2CInstance i2c, PinNumber sda, PinNumber scl);

/**
 * @brief Set cursor position on the LCD display.
 *
 * @param config LCD config data.
 * @param line Line number.
 * @param pos Position on the line.
 * @return Result code.
 */
FUNCRESULT lcdSetCursor(LCDConfig *config, INT32 line, INT32 position);

/**
 * @brief Write a character to the LCD display.
 *
 * @param config LCD config data.
 * @param character Character to write.
 * @return Result code.
 */
FUNCRESULT lcdWriteChar(LCDConfig *config, CHAR character);

/**
 * @brief Write a string to the LCD display.
 *
 * @param config LCD config data.
 * @param string String to write.
 * @return Result code.
 */
FUNCRESULT lcdWriteString(LCDConfig *config, STRING string);

/**
 * @brief Clear the LCD display.
 *
 * @param config LCD config data.
 * @return Result code.
 */
FUNCRESULT lcdClear(LCDConfig *config);