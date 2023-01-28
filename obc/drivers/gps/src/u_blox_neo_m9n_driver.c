#include "drivers/gps/u_blox_neo_m9n_driver.h"

FUNCRESULT uBloxNeoM9NInit(UBloxNeoM9NConfig *config, SPIInstance spi, PinNumber miso, PinNumber mosi, PinNumber cs, PinNumber sck)
{
    config->spi = spi;
    config->cs = cs;

    spiInitAll(spi, 10000000);
    spiInitPins(spi, miso, mosi, sck, cs);

    return SUC_OK;
}

BYTE __uBloxNeoM9NRead(UBloxNeoM9NConfig *config)
{
    BYTE data;

    gpioSetPinState(config->cs, GPIO_LOW);
    spiReadBlocking(config->spi, 0, &data, 1);
    gpioSetPinState(config->cs, GPIO_HIGH);

    return data;
}

VOID __uBloxNeoM9NWrite(UBloxNeoM9NConfig *config, BYTE data)
{
    gpioSetPinState(config->cs, GPIO_LOW);
    spiWriteBlocking(config->spi, &data, 1);
    gpioSetPinState(config->cs, GPIO_HIGH);
}