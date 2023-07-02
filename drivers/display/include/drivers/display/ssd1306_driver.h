#pragma once

/**
 * REF: https://github.com/daschr/pico-ssd1306
 */

#include <obc/api.h>
#include "drivers/gpio/i2c_driver.h"

/**
 * @brief defines commands used in ssd1306
 */
typedef enum SSD1306Command
{
    SET_CONTRAST = 0x81,
    SET_ENTIRE_ON = 0xA4,
    SET_NORM_INV = 0xA6,
    SET_DISP = 0xAE,
    SET_MEM_ADDR = 0x20,
    SET_COL_ADDR = 0x21,
    SET_PAGE_ADDR = 0x22,
    SET_DISP_START_LINE = 0x40,
    SET_SEG_REMAP = 0xA0,
    SET_MUX_RATIO = 0xA8,
    SET_COM_OUT_DIR = 0xC0,
    SET_DISP_OFFSET = 0xD3,
    SET_COM_PIN_CFG = 0xDA,
    SET_DISP_CLK_DIV = 0xD5,
    SET_PRECHARGE = 0xD9,
    SET_VCOM_DESEL = 0xDB,
    SET_CHARGE_PUMP = 0x8D,
} SSD1306Command;

/**
 * @brief holds the configuration
 */
typedef struct SSD1306Data
{
    UINT8 width;      /** Width of display */
    UINT8 height;     /** Height of display */
    UINT8 pages;      /** Stores pages of display (calculated on initialization */
    BYTE address;     /** I2C address of display */
    I2CInstance i2c;  /** I2C connection instance */
    BOOL externalVCC; /** Whether display uses external VCC */
    BYTE *buffer;     /** Display buffer */
    SIZE bufsize;     /** Buffer size */
} SSD1306Data;

/**
 * @brief Initialize display
 *
 * @param config Pointer to instance of SSD1306Data
 * @param width Width of display
 * @param height Height of display
 * @param address I2C address of display
 * @param i2c Instance of i2c connection
 * @param sda SDA pin
 * @param scl SCL pin
 * @return Result code
 */
FUNCRESULT ssd1306Init(SSD1306Data *config, UINT8 width, UINT8 height, BYTE address, I2CInstance i2c, PinNumber sda, PinNumber scl);

/**
 * @brief Turn off display
 *
 * @param config Instance of display
 * @return Result code
 */
FUNCRESULT ssd1306Poweroff(SSD1306Data *config);

/**
 * @brief Turn on display
 *
 * @param config Instance of display
 * @return Result code
 */
FUNCRESULT ssd1306Poweron(SSD1306Data *config);

/**
 * @brief Set contrast of display
 *
 * @param config Instance of display
 * @param val Contrast
 * @return Result code
 */
FUNCRESULT ssd1306Contrast(SSD1306Data *config, BOOL val);

/**
 * @brief Set invert display
 *
 * @param config Instance of display
 * @param inv inv==0: disable inverting, inv!=0: invert
 * @return Result code
 */
FUNCRESULT ssd1306Invert(SSD1306Data *config, BOOL inv);

/**
 * @brief Display buffer, should be called on change
 *
 * @param config Instance of display
 * @return Result code
 */
FUNCRESULT ssd1306Show(SSD1306Data *config);

/**
 * @brief Clear display buffer
 *
 * @param config Instance of display
 * @return Result code
 */
FUNCRESULT ssd1306Clear(SSD1306Data *config);

/**
 * @brief Draw pixel on buffer
 *
 * @param config Instance of display
 * @param x X position
 * @param y Y position
 * @return Result code
 */
FUNCRESULT ssd1306DrawPixel(SSD1306Data *config, UINT32 x, UINT32 y);

/**
 * @brief Draw pixel on buffer
 *
 * @param config Instance of display
 * @param x1 X position of starting point
 * @param y1 Y position of starting point
 * @param x2 X position of end point
 * @param y2 Y position of end point
 * @return Result code
 */
FUNCRESULT ssd1306DrawLine(SSD1306Data *config, UINT32 x1, UINT32 y1, UINT32 x2, UINT32 y2);

/**
 * @brief Draw filled square at given position with given size
 *
 * @param config Instance of display
 * @param x X position of starting point
 * @param y Y position of starting point
 * @param width Width of square
 * @param height Height of square
 * @return Result code
 */
FUNCRESULT ssd1306DrawSquare(SSD1306Data *config, UINT32 x, UINT32 y, UINT32 width, UINT32 height);

/**
 * @brief Draw empty square at given position with given size
 *
 * @param config Instance of display
 * @param x X position of starting point
 * @param y Y position of starting point
 * @param width Width of square
 * @param height Height of square
 * @return Result code
 */
FUNCRESULT ssd13606DrawEmptySquare(SSD1306Data *config, UINT32 x, UINT32 y, UINT32 width, UINT32 height);

/**
 * @brief Draw monochrome bitmap with offset
 *
 * @param config Instance of display
 * @param data Image data (whole file)
 * @param size Size of image data in bytes
 * @param x_offset Offset of horizontal coordinate
 * @param y_offset Offset of vertical coordinate
 * @return Result code
 */
FUNCRESULT ssd1306BmpShowImageWithOffset(SSD1306Data *config, const BYTE *data, SIZE size, UINT32 x_offset, UINT32 y_offset);

/**
 * @brief Draw monochrome bitmap
 *
 * @param config Instance of display
 * @param data Image data (whole file)
 * @param size Size of image data in bytes
 * @return Result code
 */
FUNCRESULT ssd1306BmpShowImage(SSD1306Data *config, const BYTE *data, SIZE size);

/**
 * @brief Draw char with given font
 *
 * @param config Instance of display
 * @param x X starting position of char
 * @param y Y starting position of char
 * @param scale Scale font to n times of original size (default should be 1)
 * @param font Pointer to font
 * @param c Character to draw
 * @return Result code
 */
FUNCRESULT ssd1306DrawCharWithFont(SSD1306Data *config, UINT32 x, UINT32 y, UINT32 scale, const BYTE *font, CHAR c);

/**
 * @brief Draw char with builtin font
 *
 * @param config Instance of display
 * @param x X starting position of char
 * @param y Y starting position of char
 * @param scale Scale font to n times of original size (default should be 1)
 * @param c Character to draw
 * @return Result code
 */
FUNCRESULT ssd1306DrawChar(SSD1306Data *config, UINT32 x, UINT32 y, UINT32 scale, CHAR c);

/**
 * @brief Draw string with given font
 *
 * @param config Instance of display
 * @param x X starting position of text
 * @param y Y starting position of text
 * @param scale Scale font to n times of original size (default should be 1)
 * @param font Pointer to font
 * @param s Text to draw
 * @return Result code
 */
FUNCRESULT ssd1306DrawStringWithFont(SSD1306Data *config, UINT32 x, UINT32 y, UINT32 scale, const BYTE *font, STRING s);

/**
 * @brief Draw string with builtin font
 *
 * @param config Instance of display
 * @param x X starting position of text
 * @param y Y starting position of text
 * @param scale Scale font to n times of original size (default should be 1)
 * @param s Text to draw
 * @return Result code
 */
FUNCRESULT ssd1306DrawString(SSD1306Data *config, UINT32 x, UINT32 y, UINT32 scale, STRING s);