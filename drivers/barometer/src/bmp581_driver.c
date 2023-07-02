#include "drivers/barometer/bmp581_driver.h"

#define I2C_ADDRESS 0x46

FUNCRESULT bmp581InitSPI(BMP581Config *config, SPIInstance spi, PinNumber miso, PinNumber mosi, PinNumber cs, PinNumber sck)
{
    config->useSPI = TRUE;
    config->spi = spi;
    config->cs = cs;

    return spiInitPins(spi, miso, mosi, cs, sck);
}

FUNCRESULT bmp581InitI2C(BMP581Config *config, I2CInstance i2c, PinNumber sda, PinNumber scl)
{
    config->useSPI = FALSE;
    config->i2c = i2c;

    return i2cInitPins(i2c, sda, scl);
}

BOOL __bmp581WriteRegField(BMP581Config *config, BYTE address, UINT8 length, UINT8 offset, BYTE value)
{
    BYTE data = __bmp581ReadReg(config, address);

    BYTE mask = 0xFF;
    mask >>= offset;
    mask <<= offset;
    mask <<= 8 - offset - length;
    mask >>= 8 - offset - length;

    data &= ~mask;
    data |= (value << offset);

    __bmp581WriteReg(config, address, data);

#if OBC_DEBUG_MODE
    BYTE read = __bmp581ReadReg(config, address);

    if (read != data)
    {
        return FALSE;
    }
#endif

    return TRUE;
}

BYTE __bmp581ReadReg(BMP581Config *config, BYTE address)
{
    BYTE data;

    address |= 0x80;

    if (config->useSPI)
    {
        gpioSetPinState(config->cs, GPIO_LOW);

        spiWriteBlocking(config->spi, &address, 1);
        spiReadBlocking(config->spi, 0, &data, 1);

        gpioSetPinState(config->cs, GPIO_HIGH);
    }
    else
    {
        i2cWriteBlocking(config->i2c, I2C_ADDRESS, &address, 1, FALSE);
        i2cReadBlocking(config->i2c, I2C_ADDRESS, &data, 1, FALSE);
    }

    return data;
}

VOID __bmp581ReadRegs(BMP581Config *config, BYTE address, BYTE *buffer, SIZE count)
{
    address |= 0x80;

    if (config->useSPI)
    {
        gpioSetPinState(config->cs, GPIO_LOW);

        spiWriteBlocking(config->spi, &address, 1);
        spiReadBlocking(config->spi, 0, buffer, count);

        gpioSetPinState(config->cs, GPIO_HIGH);
    }
    else
    {
        i2cWriteBlocking(config->i2c, I2C_ADDRESS, &address, 1, FALSE);
        i2cReadBlocking(config->i2c, I2C_ADDRESS, buffer, count, FALSE);
    }
}

VOID __bmp581WriteReg(BMP581Config *config, BYTE address, BYTE data)
{
    address &= 0x7F;

    if (config->useSPI)
    {
        gpioSetPinState(config->cs, GPIO_LOW);

        spiWriteBlocking(config->spi, &address, 1);
        spiWriteBlocking(config->spi, &data, 1);

        gpioSetPinState(config->cs, GPIO_HIGH);
    }
    else
    {
        i2cWriteBlocking(config->i2c, I2C_ADDRESS, &address, 1, FALSE);
        i2cWriteBlocking(config->i2c, I2C_ADDRESS, &data, 1, FALSE);
    }
}