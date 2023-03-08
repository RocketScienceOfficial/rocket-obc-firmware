#include "drivers/display/ssd1306_driver.h"
#include "drivers/gpio/gpio_driver.h"
#include <pico/stdlib.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

static const BYTE font_8x5[] =
    {
        8,
        5,
        0x00,
        0x00,
        0x00,
        0x00,
        0x00,
        0x00,
        0x00,
        0x5F,
        0x00,
        0x00,
        0x00,
        0x07,
        0x00,
        0x07,
        0x00,
        0x14,
        0x7F,
        0x14,
        0x7F,
        0x14,
        0x24,
        0x2A,
        0x7F,
        0x2A,
        0x12,
        0x23,
        0x13,
        0x08,
        0x64,
        0x62,
        0x36,
        0x49,
        0x56,
        0x20,
        0x50,
        0x00,
        0x08,
        0x07,
        0x03,
        0x00,
        0x00,
        0x1C,
        0x22,
        0x41,
        0x00,
        0x00,
        0x41,
        0x22,
        0x1C,
        0x00,
        0x2A,
        0x1C,
        0x7F,
        0x1C,
        0x2A,
        0x08,
        0x08,
        0x3E,
        0x08,
        0x08,
        0x00,
        0x80,
        0x70,
        0x30,
        0x00,
        0x08,
        0x08,
        0x08,
        0x08,
        0x08,
        0x00,
        0x00,
        0x60,
        0x60,
        0x00,
        0x20,
        0x10,
        0x08,
        0x04,
        0x02,
        0x3E,
        0x51,
        0x49,
        0x45,
        0x3E,
        0x00,
        0x42,
        0x7F,
        0x40,
        0x00,
        0x72,
        0x49,
        0x49,
        0x49,
        0x46,
        0x21,
        0x41,
        0x49,
        0x4D,
        0x33,
        0x18,
        0x14,
        0x12,
        0x7F,
        0x10,
        0x27,
        0x45,
        0x45,
        0x45,
        0x39,
        0x3C,
        0x4A,
        0x49,
        0x49,
        0x31,
        0x41,
        0x21,
        0x11,
        0x09,
        0x07,
        0x36,
        0x49,
        0x49,
        0x49,
        0x36,
        0x46,
        0x49,
        0x49,
        0x29,
        0x1E,
        0x00,
        0x00,
        0x14,
        0x00,
        0x00,
        0x00,
        0x40,
        0x34,
        0x00,
        0x00,
        0x00,
        0x08,
        0x14,
        0x22,
        0x41,
        0x14,
        0x14,
        0x14,
        0x14,
        0x14,
        0x00,
        0x41,
        0x22,
        0x14,
        0x08,
        0x02,
        0x01,
        0x59,
        0x09,
        0x06,
        0x3E,
        0x41,
        0x5D,
        0x59,
        0x4E,
        0x7C,
        0x12,
        0x11,
        0x12,
        0x7C,
        0x7F,
        0x49,
        0x49,
        0x49,
        0x36,
        0x3E,
        0x41,
        0x41,
        0x41,
        0x22,
        0x7F,
        0x41,
        0x41,
        0x41,
        0x3E,
        0x7F,
        0x49,
        0x49,
        0x49,
        0x41,
        0x7F,
        0x09,
        0x09,
        0x09,
        0x01,
        0x3E,
        0x41,
        0x41,
        0x51,
        0x73,
        0x7F,
        0x08,
        0x08,
        0x08,
        0x7F,
        0x00,
        0x41,
        0x7F,
        0x41,
        0x00,
        0x20,
        0x40,
        0x41,
        0x3F,
        0x01,
        0x7F,
        0x08,
        0x14,
        0x22,
        0x41,
        0x7F,
        0x40,
        0x40,
        0x40,
        0x40,
        0x7F,
        0x02,
        0x1C,
        0x02,
        0x7F,
        0x7F,
        0x04,
        0x08,
        0x10,
        0x7F,
        0x3E,
        0x41,
        0x41,
        0x41,
        0x3E,
        0x7F,
        0x09,
        0x09,
        0x09,
        0x06,
        0x3E,
        0x41,
        0x51,
        0x21,
        0x5E,
        0x7F,
        0x09,
        0x19,
        0x29,
        0x46,
        0x26,
        0x49,
        0x49,
        0x49,
        0x32,
        0x03,
        0x01,
        0x7F,
        0x01,
        0x03,
        0x3F,
        0x40,
        0x40,
        0x40,
        0x3F,
        0x1F,
        0x20,
        0x40,
        0x20,
        0x1F,
        0x3F,
        0x40,
        0x38,
        0x40,
        0x3F,
        0x63,
        0x14,
        0x08,
        0x14,
        0x63,
        0x03,
        0x04,
        0x78,
        0x04,
        0x03,
        0x61,
        0x59,
        0x49,
        0x4D,
        0x43,
        0x00,
        0x7F,
        0x41,
        0x41,
        0x41,
        0x02,
        0x04,
        0x08,
        0x10,
        0x20,
        0x00,
        0x41,
        0x41,
        0x41,
        0x7F,
        0x04,
        0x02,
        0x01,
        0x02,
        0x04,
        0x40,
        0x40,
        0x40,
        0x40,
        0x40,
        0x00,
        0x03,
        0x07,
        0x08,
        0x00,
        0x20,
        0x54,
        0x54,
        0x78,
        0x40,
        0x7F,
        0x28,
        0x44,
        0x44,
        0x38,
        0x38,
        0x44,
        0x44,
        0x44,
        0x28,
        0x38,
        0x44,
        0x44,
        0x28,
        0x7F,
        0x38,
        0x54,
        0x54,
        0x54,
        0x18,
        0x00,
        0x08,
        0x7E,
        0x09,
        0x02,
        0x18,
        0xA4,
        0xA4,
        0x9C,
        0x78,
        0x7F,
        0x08,
        0x04,
        0x04,
        0x78,
        0x00,
        0x44,
        0x7D,
        0x40,
        0x00,
        0x20,
        0x40,
        0x40,
        0x3D,
        0x00,
        0x7F,
        0x10,
        0x28,
        0x44,
        0x00,
        0x00,
        0x41,
        0x7F,
        0x40,
        0x00,
        0x7C,
        0x04,
        0x78,
        0x04,
        0x78,
        0x7C,
        0x08,
        0x04,
        0x04,
        0x78,
        0x38,
        0x44,
        0x44,
        0x44,
        0x38,
        0xFC,
        0x18,
        0x24,
        0x24,
        0x18,
        0x18,
        0x24,
        0x24,
        0x18,
        0xFC,
        0x7C,
        0x08,
        0x04,
        0x04,
        0x08,
        0x48,
        0x54,
        0x54,
        0x54,
        0x24,
        0x04,
        0x04,
        0x3F,
        0x44,
        0x24,
        0x3C,
        0x40,
        0x40,
        0x20,
        0x7C,
        0x1C,
        0x20,
        0x40,
        0x20,
        0x1C,
        0x3C,
        0x40,
        0x30,
        0x40,
        0x3C,
        0x44,
        0x28,
        0x10,
        0x28,
        0x44,
        0x4C,
        0x90,
        0x90,
        0x90,
        0x7C,
        0x44,
        0x64,
        0x54,
        0x4C,
        0x44,
        0x00,
        0x08,
        0x36,
        0x41,
        0x00,
        0x00,
        0x00,
        0x77,
        0x00,
        0x00,
        0x00,
        0x41,
        0x36,
        0x08,
        0x00,
        0x02,
        0x01,
        0x02,
        0x04,
        0x02,
};

static void __swap(UINT32 *a, UINT32 *b)
{
    UINT32 *t = a;

    *a = *b;
    *b = *t;
}

static BOOL __fancyWrite(SSD1306Data *config, const BYTE *src, SIZE len)
{
    return FUNCSUCCESS(i2cWriteBlocking(config->i2c, config->address, src, len, FALSE));
}

static BOOL __ssd1306Write(SSD1306Data *config, BYTE val)
{
    BYTE d[2] = {0x00, val};

    return __fancyWrite(config, d, 2);
}

static UINT32 __ssd1306BmpGetVal(const BYTE *data, const SIZE offset, BYTE size)
{
    switch (size)
    {
    case 1:
        return data[offset];
    case 2:
        return data[offset] | (data[offset + 1] << 8);
    case 4:
        return data[offset] | (data[offset + 1] << 8) | (data[offset + 2] << 16) | (data[offset + 3] << 24);
    default:
        __builtin_unreachable();
    }

    __builtin_unreachable();
}

FUNCRESULT ssd1306Init(SSD1306Data *config, UINT8 width, UINT8 height, BYTE address, I2CInstance i2c, PinNumber sda, PinNumber scl)
{
    config->width = width;
    config->height = height;
    config->pages = height / 8;
    config->address = address;
    config->i2c = i2c;
    config->bufsize = (config->pages) * (config->width);

    if (FUNCFAILED(i2cInitPins(i2c, sda, scl)))
    {
        return ERR_FAIL;
    }

    if ((config->buffer = malloc(config->bufsize + 1)) == NULL)
    {
        config->bufsize = 0;

        return ERR_UNINITIALIZED;
    }

    ++(config->buffer);

    // from https://github.com/makerportal/rpi-pico-ssd1306
    BYTE cmds[] = {
        SET_DISP,
        SET_DISP_CLK_DIV,
        0x80,
        SET_MUX_RATIO,
        height - 1,
        SET_DISP_OFFSET,
        0x00,
        SET_DISP_START_LINE,
        SET_CHARGE_PUMP,
        config->externalVCC ? 0x10 : 0x14,
        SET_SEG_REMAP | 0x01,
        SET_COM_OUT_DIR | 0x08,
        SET_COM_PIN_CFG,
        width > 2 * height ? 0x02 : 0x12,
        SET_CONTRAST,
        0xff,
        SET_PRECHARGE,
        config->externalVCC ? 0x22 : 0xF1,
        SET_VCOM_DESEL,
        0x30,
        SET_ENTIRE_ON,
        SET_NORM_INV,
        SET_DISP | 0x01,
        SET_MEM_ADDR,
        0x00,
    };

    for (SIZE i = 0; i < sizeof(cmds); ++i)
    {
        if (!__ssd1306Write(config, cmds[i]))
        {
            return ERR_FAIL;
        }
    }

    return SUC_OK;
}

FUNCRESULT ssd1306Poweroff(SSD1306Data *config)
{
    return __ssd1306Write(config, SET_DISP | 0x00) ? SUC_OK : ERR_FAIL;
}

FUNCRESULT ssd1306Poweron(SSD1306Data *config)
{
    return __ssd1306Write(config, SET_DISP | 0x01) ? SUC_OK : ERR_FAIL;
}

FUNCRESULT ssd1306Contrast(SSD1306Data *config, BOOL val)
{
    return __ssd1306Write(config, SET_CONTRAST) && __ssd1306Write(config, (BYTE)val) ? SUC_OK : ERR_FAIL;
}

FUNCRESULT ssd1306Invert(SSD1306Data *config, BOOL inv)
{
    return __ssd1306Write(config, SET_NORM_INV | ((BYTE)inv & 1)) ? SUC_OK : ERR_FAIL;
}

FUNCRESULT ssd1306Show(SSD1306Data *config)
{
    BYTE payload[] = {SET_COL_ADDR, 0, config->width - 1, SET_PAGE_ADDR, 0, config->pages - 1};

    if (config->width == 64)
    {
        payload[1] += 32;
        payload[2] += 32;
    }

    for (SIZE i = 0; i < sizeof(payload); ++i)
    {
        if (!__ssd1306Write(config, payload[i]))
        {
            return ERR_FAIL;
        }
    }

    *(config->buffer - 1) = 0x40;

    return __fancyWrite(config, config->buffer - 1, config->bufsize + 1) ? SUC_OK : ERR_FAIL;
}

FUNCRESULT ssd1306Clear(SSD1306Data *config)
{
    memset(config->buffer, 0, config->bufsize);

    return SUC_OK;
}

FUNCRESULT ssd1306DrawPixel(SSD1306Data *config, UINT32 x, UINT32 y)
{
    if (x >= config->width || y >= config->height)
    {
        return ERR_INVALIDARG;
    }

    config->buffer[x + config->width * (y >> 3)] |= 0x1 << (y & 0x07);

    return SUC_OK;
}

FUNCRESULT ssd1306DrawLine(SSD1306Data *config, UINT32 x1, UINT32 y1, UINT32 x2, UINT32 y2)
{
    if (x1 > x2)
    {
        __swap(&x1, &x2);
        __swap(&y1, &y2);
    }

    if (x1 == x2)
    {
        if (y1 > y2)
        {
            __swap(&y1, &y2);
        }

        for (UINT32 i = y1; i <= y2; ++i)
        {
            if (FUNCFAILED(ssd1306DrawPixel(config, x1, i)))
            {
                return ERR_FAIL;
            }
        }
    }
    else
    {
        FLOAT m = (FLOAT)((INT32)y2 - (INT32)y1) / (FLOAT)((INT32)x2 - (INT32)x1);

        for (UINT32 i = x1; i <= x2; ++i)
        {
            FLOAT y = m * (FLOAT)((INT32)i - (INT32)x1) + (FLOAT)y1;

            if (FUNCFAILED(ssd1306DrawPixel(config, i, (UINT32)y)))
            {
                return ERR_FAIL;
            }
        }
    }

    return SUC_OK;
}

FUNCRESULT ssd1306DrawSquare(SSD1306Data *config, UINT32 x, UINT32 y, UINT32 width, UINT32 height)
{
    for (UINT32 i = 0; i < width; ++i)
    {
        for (UINT32 j = 0; j < height; ++j)
        {
            if (FUNCFAILED(ssd1306DrawPixel(config, x + i, y + j)))
            {
                return ERR_FAIL;
            }
        }
    }

    return SUC_OK;
}

FUNCRESULT ssd13606DrawEmptySquare(SSD1306Data *config, UINT32 x, UINT32 y, UINT32 width, UINT32 height)
{
    return FUNCSUCCESS(ssd1306DrawLine(config, x, y, x + width, y)) &&
                   FUNCSUCCESS(ssd1306DrawLine(config, x, y + height, x + width, y + height)) &&
                   FUNCSUCCESS(ssd1306DrawLine(config, x, y, x, y + height)) &&
                   FUNCSUCCESS(ssd1306DrawLine(config, x + width, y, x + width, y + height))
               ? SUC_OK
               : ERR_FAIL;
}

FUNCRESULT ssd1306BmpShowImageWithOffset(SSD1306Data *config, const BYTE *data, SIZE size, UINT32 x_offset, UINT32 y_offset)
{
    if (size < 54)
    {
        return ERR_INVALIDARG;
    }

    const UINT32 bfOffBits = __ssd1306BmpGetVal(data, 10, 4);
    const UINT32 biSize = __ssd1306BmpGetVal(data, 14, 4);
    const INT32 biWidth = (INT32)__ssd1306BmpGetVal(data, 18, 4);
    const INT32 biHeight = (INT32)__ssd1306BmpGetVal(data, 22, 4);
    const UINT16 biBitCount = (INT16)__ssd1306BmpGetVal(data, 28, 2);
    const UINT32 biCompression = __ssd1306BmpGetVal(data, 30, 4);

    if (biBitCount != 1) // image not monochrome
    {
        return ERR_FAIL;
    }

    if (biCompression != 0) // image compressed
    {
        return ERR_FAIL;
    }

    const INT32 table_start = 14 + biSize;

    BYTE color_val;

    for (BYTE i = 0; i < 2; ++i)
    {
        if (!((data[table_start + i * 4] << 16) | (data[table_start + i * 4 + 1] << 8) | data[table_start + i * 4 + 2]))
        {
            color_val = i;

            break;
        }
    }

    UINT32 bytes_per_line = (biWidth / 8) + (biWidth & 7 ? 1 : 0);

    if (bytes_per_line & 3)
    {
        bytes_per_line = (bytes_per_line ^ (bytes_per_line & 3)) + 4;
    }

    const BYTE *img_data = data + bfOffBits;

    INT32 step = biHeight > 0 ? -1 : 1;
    INT32 border = biHeight > 0 ? -1 : biHeight;

    for (UINT32 y = biHeight > 0 ? biHeight - 1 : 0; y != border; y += step)
    {
        for (UINT32 x = 0; x < biWidth; ++x)
        {
            if (((img_data[x >> 3] >> (7 - (x & 7))) & 1) == color_val)
            {
                if (FUNCFAILED(ssd1306DrawPixel(config, x_offset + x, y_offset + y)))
                {
                    return ERR_FAIL;
                }
            }
        }

        img_data += bytes_per_line;
    }

    return SUC_OK;
}

FUNCRESULT ssd1306BmpShowImage(SSD1306Data *config, const BYTE *data, SIZE size)
{
    return ssd1306BmpShowImageWithOffset(config, data, size, 0, 0);
}

FUNCRESULT ssd1306DrawCharWithFont(SSD1306Data *config, UINT32 x, UINT32 y, UINT32 scale, const BYTE *font, CHAR c)
{
    if (c > '~')
    {
        return ERR_INVALIDARG;
    }

    for (UINT8 i = 0; i < font[1]; ++i)
    {
        UINT8 line = (UINT8)(font[(c - 0x20) * font[1] + i + 2]);

        for (INT8 j = 0; j < font[0]; ++j, line >>= 1)
        {
            if (line & 1)
            {
                if (FUNCFAILED(ssd1306DrawSquare(config, x + i * scale, y + j * scale, scale, scale)))
                {
                    return ERR_FAIL;
                }
            }
        }
    }

    return SUC_OK;
}

FUNCRESULT ssd1306DrawChar(SSD1306Data *config, UINT32 x, UINT32 y, UINT32 scale, CHAR c)
{
    return ssd1306DrawCharWithFont(config, x, y, scale, font_8x5, c);
}

FUNCRESULT ssd1306DrawStringWithFont(SSD1306Data *config, UINT32 x, UINT32 y, UINT32 scale, const BYTE *font, STRING s)
{
    for (INT32 x_n = x; *s; x_n += font[0] * scale)
    {
        if (FUNCFAILED(ssd1306DrawCharWithFont(config, x_n, y, scale, font, *(s++))))
        {
            return ERR_FAIL;
        }
    }
}

FUNCRESULT ssd1306DrawString(SSD1306Data *config, UINT32 x, UINT32 y, UINT32 scale, STRING s)
{
    return ssd1306DrawStringWithFont(config, x, y, scale, font_8x5, s);
}