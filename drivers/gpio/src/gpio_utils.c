#include "drivers/gpio/gpio_utils.h"

BOOL gpioWriteRegField(GPIOCommunicationConfig *config, BYTE address, UINT8 length, UINT8 offset, BYTE value)
{
    BYTE data = gpioReadReg(config, address);

    BYTE mask = 0xFF;
    mask >>= offset;
    mask <<= offset;
    mask <<= 8 - offset - length;
    mask >>= 8 - offset - length;

    data &= ~mask;
    data |= (value << offset);

    gpioWriteReg(config, address, data);

#if OBC_DEBUG_MODE
    BYTE read = gpioReadReg(config, address);

    if (read != data)
    {
        return FALSE;
    }
#endif

    return TRUE;
}

BYTE gpioReadReg(GPIOCommunicationConfig *config, BYTE address)
{
    BYTE data;

    address |= config->readMask;

    if (config->protocol == GPIO_PROTOCOL_I2C)
    {
        i2cWriteBlocking(config->i2c, config->i2cAddress, &address, 1, FALSE);
        i2cReadBlocking(config->i2c, config->i2cAddress, &data, 1, FALSE);
    }
    else if (config->protocol == GPIO_PROTOCOL_SPI)
    {
        gpioSetPinState(config->cs, GPIO_LOW);

        spiWriteBlocking(config->spi, &address, 1);
        spiReadBlocking(config->spi, 0, &data, 1);

        gpioSetPinState(config->cs, GPIO_HIGH);
    }

    return data;
}

VOID gpioReadRegs(GPIOCommunicationConfig *config, BYTE address, BYTE *buffer, SIZE count)
{
    address |= config->multipleReadMask;

    if (config->protocol == GPIO_PROTOCOL_I2C)
    {
        i2cWriteBlocking(config->i2c, config->i2cAddress, &address, 1, FALSE);
        i2cReadBlocking(config->i2c, config->i2cAddress, buffer, count, FALSE);
    }
    else if (config->protocol == GPIO_PROTOCOL_SPI)
    {
        gpioSetPinState(config->cs, GPIO_LOW);

        spiWriteBlocking(config->spi, &address, 1);
        spiReadBlocking(config->spi, 0, buffer, count);

        gpioSetPinState(config->cs, GPIO_HIGH);
    }
}

VOID gpioWriteReg(GPIOCommunicationConfig *config, BYTE address, BYTE value)
{
    address &= config->writeMask;

    if (config->protocol == GPIO_PROTOCOL_I2C)
    {
        i2cWriteBlocking(config->i2c, config->i2cAddress, &address, 1, FALSE);
        i2cWriteBlocking(config->i2c, config->i2cAddress, &value, 1, FALSE);
    }
    else if (config->protocol == GPIO_PROTOCOL_SPI)
    {
        gpioSetPinState(config->cs, GPIO_LOW);

        spiWriteBlocking(config->spi, &address, 1);
        spiWriteBlocking(config->spi, &value, 1);

        gpioSetPinState(config->cs, GPIO_HIGH);
    }
}

BYTE gpioSingleRead(GPIOCommunicationConfig *config)
{
    BYTE data;

    if (config->protocol == GPIO_PROTOCOL_I2C)
    {
        i2cReadBlocking(config->i2c, config->i2cAddress, &data, 1, FALSE);
    }
    else if (config->protocol == GPIO_PROTOCOL_SPI)
    {
        gpioSetPinState(config->cs, GPIO_LOW);

        spiReadBlocking(config->spi, 0, &data, 1);

        gpioSetPinState(config->cs, GPIO_HIGH);
    }

    return data;
}