#include "drivers/accelerometer/h3lis331dl_driver.h"

#define CTRL_REG1 0x20
#define CTRL_REG2 0x21
#define CTRL_REG3 0x22
#define CTRL_REG4 0x23
#define CTRL_REG5 0x24
#define STATUS_REG 0x27
#define OUT_X_L 0x28
#define OUT_X_H 0x29
#define OUT_Y_L 0x2A
#define OUT_Y_H 0x2B
#define OUT_Z_L 0x2C
#define OUT_Z_H 0x2D

FUNCRESULT h3lis331dlInit(H3lis331dlConfig *config, SPIInstance spi, PinNumber miso, PinNumber mosi, PinNumber cs, PinNumber sck)
{
    config->spi = spi;
    config->cs = cs;

    spiInitPins(spi, miso, mosi, sck, cs);

    return SUC_OK;
}

FUNCRESULT h3lis331dlRead(H3lis331dlConfig *config, H3lis331dlData *data)
{
}

BYTE __h3lis331dlReadReg(H3lis331dlConfig *config, BYTE address)
{
    BYTE data;
    BYTE newAddress = 1 << 7 | address << 2;

    gpioSetPinState(config->cs, GPIO_LOW);

    spiWriteBlocking(config->spi, &newAddress, 1);
    spiReadBlocking(config->spi, 0, &data, 1);

    gpioSetPinState(config->cs, GPIO_HIGH);

    return data;
}

VOID __h3lis331dlWriteReg(H3lis331dlConfig *config, BYTE address, BYTE data)
{
    BYTE newAddress = address << 2;

    gpioSetPinState(config->cs, GPIO_LOW);

    spiWriteBlocking(config->spi, &newAddress, 1);
    spiReadBlocking(config->spi, 0, &data, 1);

    gpioSetPinState(config->cs, GPIO_HIGH);
}