#include "drivers/accelerometer/h3lis331dl_driver.h"

#define WHO_AM_I 0x0F
#define CTRL_REG1 0x20
#define CTRL_REG2 0x21
#define CTRL_REG3 0x22
#define CTRL_REG4 0x23
#define CTRL_REG5 0x24
#define HP_FILTER_TEST 0x25
#define REFERENCE 0x26
#define STATUS_REG 0x27
#define OUT_X_L 0x28
#define OUT_X_H 0x29
#define OUT_Y_L 0x2A
#define OUT_Y_H 0x2B
#define OUT_Z_L 0x2C
#define OUT_Z_H 0x2D
#define INT_1_CFG 0x30
#define INT_1_SRC 0x31
#define INT_1_THS 0x32
#define INT_1_DURATION 0x33
#define INT_2_CFG 0x34
#define INT_2_SRC 0x35
#define INT_2_THS 0x36
#define INT_2_DURATION 0x37

#define RESOLUTION_DIVIDER 32768.0f

FUNCRESULT h3lis331dlInit(H3lis331dlConfig *config, SPIInstance spi, PinNumber miso, PinNumber mosi, PinNumber cs, PinNumber sck)
{
    config->spi = spi;
    config->cs = cs;
    config->rangeFactor = 0;

    spiInitPins(spi, miso, mosi, sck, cs);

    return SUC_OK;
}

FUNCRESULT h3lis331dlWhoAmI(H3lis331dlConfig *config, BYTE *whoami)
{
    *whoami = __h3lis331dlReadReg(config, WHO_AM_I);

    return SUC_OK;
}

FUNCRESULT h3lis331dlSetPower(H3lis331dlConfig *config, BOOL power)
{
    BYTE data = __h3lis331dlReadReg(config, CTRL_REG1);

    data &= 0x1F;

    if (power)
    {
        data |= 1 << 5;
    }

    __h3lis331dlWriteReg(config, CTRL_REG1, data);

    return SUC_OK;
}

FUNCRESULT h3lis331dlSetODR(H3lis331dlConfig *config, H3lis331dlODR odr)
{
    BYTE data = __h3lis331dlReadReg(config, CTRL_REG1);

    data &= 0xE7;
    data |= (odr << 3);

    __h3lis331dlWriteReg(config, CTRL_REG1, data);

    return SUC_OK;
}

FUNCRESULT h3lis331dlSetRange(H3lis331dlConfig *config, H3lis331dlRange range)
{
    BYTE data = __h3lis331dlReadReg(config, CTRL_REG4);

    data &= 0xCF;
    data |= (range << 4);

    __h3lis331dlWriteReg(config, CTRL_REG4, data);

    switch (range)
    {
    case H3LIS331DL_RANGE_100G:
        config->rangeFactor = 100.0f / RESOLUTION_DIVIDER;
        break;
    case H3LIS331DL_RANGE_200G:
        config->rangeFactor = 200.0f / RESOLUTION_DIVIDER;
        break;
    case H3LIS331DL_RANGE_400G:
        config->rangeFactor = 400.0f / RESOLUTION_DIVIDER;
        break;
    default:
        break;
    }

    return SUC_OK;
}

FUNCRESULT h3lis33dlStatus(H3lis331dlConfig *config, BYTE *status)
{
    *status = __h3lis331dlReadReg(config, STATUS_REG);

    return SUC_OK;
}

FUNCRESULT h3lis331dlRead(H3lis331dlConfig *config, H3lis331dlData *data)
{
    BYTE buffer[6];

    __h3lis331dlReadRegs(config, OUT_X_L, buffer, 6);

    INT16 x = (INT16)((buffer[1] << 8) | buffer[0]);
    INT16 y = (INT16)((buffer[3] << 8) | buffer[2]);
    INT16 z = (INT16)((buffer[5] << 8) | buffer[4]);

    data->accel.x = (FLOAT)x * config->rangeFactor;
    data->accel.y = (FLOAT)y * config->rangeFactor;
    data->accel.z = (FLOAT)z * config->rangeFactor;

    return SUC_OK;
}

BYTE __h3lis331dlReadReg(H3lis331dlConfig *config, BYTE address)
{
    BYTE data;

    address |= 0x80;

    gpioSetPinState(config->cs, GPIO_LOW);

    spiWriteBlocking(config->spi, &address, 1);
    spiReadBlocking(config->spi, 0, &data, 1);

    gpioSetPinState(config->cs, GPIO_HIGH);

    return data;
}

VOID __h3lis331dlReadRegs(H3lis331dlConfig *config, BYTE address, BYTE *buffer, SIZE count)
{
    address |= 0xC0;

    gpioSetPinState(config->cs, GPIO_LOW);

    spiWriteBlocking(config->spi, &address, 1);
    spiReadBlocking(config->spi, 0, buffer, count);

    gpioSetPinState(config->cs, GPIO_HIGH);
}

VOID __h3lis331dlWriteReg(H3lis331dlConfig *config, BYTE address, BYTE data)
{
    address &= 0x7F;

    gpioSetPinState(config->cs, GPIO_LOW);

    spiWriteBlocking(config->spi, &address, 1);
    spiWriteBlocking(config->spi, &data, 1);

    gpioSetPinState(config->cs, GPIO_HIGH);
}