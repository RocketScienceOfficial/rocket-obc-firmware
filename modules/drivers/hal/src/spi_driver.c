#include "drivers/gpio/spi_driver.h"
#include "hardware/spi.h"
#include "pico/stdlib.h"

static spi_inst_t *getSPI(SPIInstance spi)
{
    return (spi == 0 ? spi0 : spi1);
}

BOOL spiCheckInstance(SPIInstance spi)
{
    return spi >= 0 && spi <= 1;
}

BOOL spiCheckMISO(SPIInstance spi, PinNumber miso)
{
    if (!spiCheckInstance(spi))
    {
        return FALSE;
    }

    if (spi == 0)
    {
        return miso == 0 || miso == 4 || miso == 16;
    }
    else if (spi == 1)
    {
        return miso == 8 || miso == 12;
    }
    else
    {
        return FALSE;
    }
}

BOOL spiCheckMOSI(SPIInstance spi, PinNumber mosi)
{
    if (!spiCheckInstance(spi))
    {
        return FALSE;
    }

    if (spi == 0)
    {
        return mosi == 3 || mosi == 7 || mosi == 19;
    }
    else if (spi == 1)
    {
        return mosi == 11 || mosi == 15;
    }
    else
    {
        return FALSE;
    }
}

BOOL spiCheckSCK(SPIInstance spi, PinNumber sck)
{
    if (!spiCheckInstance(spi))
    {
        return FALSE;
    }

    if (spi == 0)
    {
        return sck == 2 || sck == 6 || sck == 18;
    }
    else if (spi == 1)
    {
        return sck == 10 || sck == 14;
    }
    else
    {
        return FALSE;
    }
}

FUNCRESULT spiInitAll(SPIInstance spi, BaudRate baudrate)
{
    if (!spiCheckInstance(spi))
    {
        return ERR_INVALIDARG;
    }

    spi_init(getSPI(spi), baudrate);

    return SUC_OK;
}

FUNCRESULT spiInitPins(SPIInstance spi, PinNumber miso, PinNumber mosi, PinNumber sck, PinNumber cs)
{
    if (!spiCheckInstance(spi) || !spiCheckMISO(spi, miso) || !spiCheckMOSI(spi, mosi) || !spiCheckSCK(spi, sck))
    {
        return ERR_INVALIDARG;
    }

    gpioInitPin(cs, GPIO_OUTPUT);
    gpioSetPinState(cs, GPIO_HIGH);

    gpioSetPinFunction(miso, GPIO_FUNCTION_SPI);
    gpioSetPinFunction(mosi, GPIO_FUNCTION_SPI);
    gpioSetPinFunction(sck, GPIO_FUNCTION_SPI);

    return SUC_OK;
}

FUNCRESULT spiWriteBlocking(SPIInstance spi, const BYTE *data, SIZE size)
{
    if (!spiCheckInstance(spi) || !data)
    {
        return ERR_INVALIDARG;
    }

    if (spi_write_blocking(getSPI(spi), data, size) < 0)
    {
        return ERR_FAIL;
    }

    return SUC_OK;
}

FUNCRESULT spiReadBlocking(SPIInstance spi, BYTE repeatedTXData, BYTE *destination, SIZE size)
{
    if (!spiCheckInstance(spi) || !destination)
    {
        return ERR_INVALIDARG;
    }

    if (spi_read_blocking(getSPI(spi), repeatedTXData, destination, size) < 0)
    {
        return ERR_FAIL;
    }

    return SUC_OK;
}

FUNCRESULT spiWriteReadBlocking(SPIInstance spi, const BYTE *data, BYTE *destination, SIZE size)
{
    if (!spiCheckInstance(spi) || !destination)
    {
        return ERR_INVALIDARG;
    }

    if (spi_write_read_blocking(getSPI(spi), data, destination, size) < 0)
    {
        return ERR_FAIL;
    }

    return SUC_OK;
}

BOOL spiCheckDevice(SPIInstance spi)
{
    if (!spiCheckInstance(spi))
    {
        return FALSE;
    }

    return spi_is_readable(getSPI(spi));
}