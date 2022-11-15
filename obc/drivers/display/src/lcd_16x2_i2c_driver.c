#include "drivers/display/lcd_16x2_i2c_driver.h"
#include "pico/stdlib.h"

const int LCD_CLEARDISPLAY = 0x01;
const int LCD_RETURNHOME = 0x02;
const int LCD_ENTRYMODESET = 0x04;
const int LCD_DISPLAYCONTROL = 0x08;
const int LCD_CURSORSHIFT = 0x10;
const int LCD_FUNCTIONSET = 0x20;
const int LCD_SETCGRAMADDR = 0x40;
const int LCD_SETDDRAMADDR = 0x80;

// flags for display entry mode
const int LCD_ENTRYSHIFTINCREMENT = 0x01;
const int LCD_ENTRYLEFT = 0x02;

// flags for display and cursor control
const int LCD_BLINKON = 0x01;
const int LCD_CURSORON = 0x02;
const int LCD_DISPLAYON = 0x04;

// flags for display and cursor shift
const int LCD_MOVERIGHT = 0x04;
const int LCD_DISPLAYMOVE = 0x08;

// flags for function set
const int LCD_5x10DOTS = 0x04;
const int LCD_2LINE = 0x08;
const int LCD_8BITMODE = 0x10;

// flag for backlight control
const int LCD_BACKLIGHT = 0x08;

const int LCD_ENABLE_BIT = 0x04;

// By default these LCD display drivers are on bus ADDRess 0x27
static int ADDR = 0x27;

// Modes for __lcdSendByte
#define LCD_CHARACTER 1
#define LCD_COMMAND 0

static BOOL __i2cWriteByte(LCDConfig *config, BYTE val)
{
    return FUNCSUCCESS(i2cWriteBlocking(config->_i2c, ADDR, &val, 1, FALSE));
}

static BOOL __lcdToggleEnable(LCDConfig *config, BYTE val)
{
    const INT32 delay_us = 600;

    BOOL result = TRUE;

    sleep_us(delay_us);
    result = result && __i2cWriteByte(config, val | LCD_ENABLE_BIT);
    sleep_us(delay_us);
    result = result && __i2cWriteByte(config, val & ~LCD_ENABLE_BIT);
    sleep_us(delay_us);

    return result;
}

static BOOL __lcdSendByte(LCDConfig *config, BYTE val, INT32 mode)
{
    BYTE high = mode | (val & 0xF0) | LCD_BACKLIGHT;
    BYTE low = mode | ((val << 4) & 0xF0) | LCD_BACKLIGHT;
    BOOL result = TRUE;

    result = result && __i2cWriteByte(config, high);
    result = result && __lcdToggleEnable(config, high);
    result = result && __i2cWriteByte(config, low);
    result = result && __lcdToggleEnable(config, low);

    return result;
}

FUNCRESULT lcdInit(LCDConfig *config, I2CInstance i2c, PinNumber sda, PinNumber scl)
{
    if (!config)
    {
        return ERR_INVALIDARG;
    }

    if (FUNCFAILED(i2cInitAll(i2c, 100 * 1000)) || FUNCFAILED(i2cInitPins(i2c, sda, scl)))
    {
        return ERR_FAIL;
    }

    BOOL result = TRUE;

    result = result && __lcdSendByte(config, 0x03, LCD_COMMAND);
    result = result && __lcdSendByte(config, 0x03, LCD_COMMAND);
    result = result && __lcdSendByte(config, 0x03, LCD_COMMAND);
    result = result && __lcdSendByte(config, 0x02, LCD_COMMAND);

    result = result && __lcdSendByte(config, LCD_ENTRYMODESET | LCD_ENTRYLEFT, LCD_COMMAND);
    result = result && __lcdSendByte(config, LCD_FUNCTIONSET | LCD_2LINE, LCD_COMMAND);
    result = result && __lcdSendByte(config, LCD_DISPLAYCONTROL | LCD_DISPLAYON, LCD_COMMAND);

    if (!result)
    {
        return ERR_UNEXPECTED;
    }

    FUNCRESULT res = lcdClear(config);

    return res;
}

FUNCRESULT lcdSetCursor(LCDConfig *config, INT32 line, INT32 position)
{
    INT32 val = (line == 0) ? 0x80 + position : 0xC0 + position;

    return __lcdSendByte(config, val, LCD_COMMAND) ? SUC_OK : ERR_FAIL;
}

FUNCRESULT lcdWriteChar(LCDConfig *config, CHAR character)
{
    return __lcdSendByte(config, character, LCD_CHARACTER) ? SUC_OK : ERR_FAIL;
}

FUNCRESULT lcdWriteString(LCDConfig *config, STRING string)
{
    while (*string)
    {
        if (FUNCFAILED(lcdWriteChar(config, *string++)))
        {
            return ERR_FAIL;
        }
    }

    return SUC_OK;
}

FUNCRESULT lcdClear(LCDConfig *config)
{
    return __lcdSendByte(config, LCD_CLEARDISPLAY, LCD_COMMAND) ? SUC_OK : ERR_FAIL;
}