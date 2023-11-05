#include "drivers/accelerometer/h3lis331dl_driver.h"

#define WHO_AM_I 0x0F
#define CTRL_REG1 0x20
#define CTRL_REG2 0x21
#define CTRL_REG3 0x22
#define CTRL_REG4 0x23
#define CTRL_REG5 0x24
#define HP_FILTER_RESET 0x25
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

#define I2C_ADDRESS 0x18
#define WHO_AM_I_VALUE 0x32
#define RESOLUTION_DIVIDER 32768.0f

#define X_OFFSET -95
#define Y_OFFSET -45
#define Z_OFFSET 420

FUNCRESULT h3lis331dlInitSPI(H3lis331dlConfig *config, SPIInstance spi, PinNumber miso, PinNumber mosi, PinNumber cs, PinNumber sck)
{
    config->gpioConfig = (GPIOCommunicationConfig){
        .protocol = GPIO_PROTOCOL_SPI,
        .spi = spi,
        .cs = cs,
        .readMask = 0x80,
        .multipleReadMask = 0xC0,
        .writeMask = 0x7F,
    };
    config->rangeFactor = 0;

    return spiInitPins(spi, miso, mosi, sck, cs);
}

FUNCRESULT h3lis331dlInitI2C(H3lis331dlConfig *config, I2CInstance i2c, PinNumber sda, PinNumber scl)
{
    config->gpioConfig = (GPIOCommunicationConfig){
        .protocol = GPIO_PROTOCOL_I2C,
        .i2c = i2c,
        .i2cAddress = I2C_ADDRESS,
        .readMask = 0x80,
        .multipleReadMask = 0xC0,
        .writeMask = 0x7F,
    };
    config->rangeFactor = 0;

    return i2cInitPins(i2c, sda, scl);
}

FUNCRESULT h3lis331dlValidateId(H3lis331dlConfig *config, BOOL *valid)
{
    *valid = gpioReadReg(&config->gpioConfig, WHO_AM_I) == WHO_AM_I_VALUE;

    return SUC_OK;
}

FUNCRESULT h3lis331dlSetPowerMode(H3lis331dlConfig *config, H3lis331dlPowerMode power)
{
    FUNC_CHECK_BOOL(gpioWriteRegField(&config->gpioConfig, CTRL_REG1, 3, 5, (BYTE)power));

    return SUC_OK;
}

FUNCRESULT h3lis331dlSetODR(H3lis331dlConfig *config, H3lis331dlODR odr)
{
    FUNC_CHECK_BOOL(gpioWriteRegField(&config->gpioConfig, CTRL_REG1, 2, 3, (BYTE)odr));

    return SUC_OK;
}

FUNCRESULT h3lis331dlSetHPCF(H3lis331dlConfig *config, H3lis331dlHPFc hpcf)
{
    FUNC_CHECK_BOOL(gpioWriteRegField(&config->gpioConfig, CTRL_REG2, 2, 0, (BYTE)hpcf));

    return SUC_OK;
}

FUNCRESULT h3lis331dlSetRange(H3lis331dlConfig *config, H3lis331dlRange range)
{
    FUNC_CHECK_BOOL(gpioWriteRegField(&config->gpioConfig, CTRL_REG4, 2, 4, (BYTE)range));

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
        return ERR_INVALIDARG;
    }

    return SUC_OK;
}

FUNCRESULT h3lis331dlRead(H3lis331dlConfig *config, vec3 *accel)
{
    BYTE buffer[6];

    gpioReadRegs(&config->gpioConfig, OUT_X_L, buffer, 6);

    INT16 x = (INT16)((buffer[1] << 8) | buffer[0]) - X_OFFSET;
    INT16 y = (INT16)((buffer[3] << 8) | buffer[2]) - Y_OFFSET;
    INT16 z = (INT16)((buffer[5] << 8) | buffer[4]) - Z_OFFSET;

    accel->x = (FLOAT)x * config->rangeFactor;
    accel->y = (FLOAT)y * config->rangeFactor;
    accel->z = (FLOAT)z * config->rangeFactor;

    return SUC_OK;
}