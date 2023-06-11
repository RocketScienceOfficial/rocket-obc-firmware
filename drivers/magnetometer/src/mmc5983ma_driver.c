#include "drivers/magnetometer/mmc5983ma_driver.h"
#include "tools/time_tracker.h"

#define X_OUT_0 0x00
#define X_OUT_1 0x01
#define Y_OUT_0 0x02
#define Y_OUT_1 0x03
#define Z_OUT_0 0x04
#define Z_OUT_1 0x05
#define XYZ_OUT_2 0x06
#define T_OUT 0x07
#define STATUS 0x08
#define INTERNAL_CONTROL_0 0x09
#define INTERNAL_CONTROL_1 0x0A
#define INTERNAL_CONTROL_2 0x0B
#define INTERNAL_CONTROL_3 0x0C
#define PRODUCT_ID 0x2F

#define PRODUCT_ID_VALID 0x0C
#define RESOLUTION 0.0625f
#define BASE_OFFSET 131072

static float HARD_IRON_CALIB[3] = {-21.11f, 11.87f, -47.88f};
static float SOFT_IRON_CALIB[3][3] = {
    {1.022f, 0.007f, 0.007f},
    {0.007f, 0.979f, -0.001f},
    {0.007f, -0.001f, 0.999f},
};

FUNCRESULT mmc5983maInit(MMC5983MAConfig *config, SPIInstance spi, PinNumber miso, PinNumber mosi, PinNumber cs, PinNumber sck)
{
    config->spi = spi;
    config->cs = cs;

    spiInitPins(spi, miso, mosi, sck, cs);

    __mmc5983maWriteReg(config, INTERNAL_CONTROL_0, 0x20);

    mmc5983maReset(config);

    return SUC_OK;
}

FUNCRESULT mmc5983ValidateId(MMC5983MAConfig *config, BOOL *valid)
{
    *valid = __mmc5983maReadReg(config, PRODUCT_ID) == PRODUCT_ID_VALID;

    return SUC_OK;
}

FUNCRESULT mmc5983maSetBandwidth(MMC5983MAConfig *config, MMC5983MABandwidth bandwidth)
{
    __mmc5983maWriteReg(config, INTERNAL_CONTROL_1, bandwidth);

    return SUC_OK;
}

FUNCRESULT mmc5983maSetContinuousModeODR(MMC5983MAConfig *config, MMC5983ODR odr)
{
    __mmc5983maWriteReg(config, INTERNAL_CONTROL_2, odr == 0 ? 0 : (0x08 | odr));

    return SUC_OK;
}

FUNCRESULT mmc5983maReset(MMC5983MAConfig *config)
{
    __mmc5983maWriteReg(config, INTERNAL_CONTROL_1, 0x80);

    sleepMiliseconds(20);

    __mmc5983maSET(config);
    __mmc5983maRESET(config);

    mmc5983maSetContinuousModeODR(config, 0);

    return SUC_OK;
}

FUNCRESULT mmc5983maCalculateOffset(MMC5983MAConfig *config, vec3 *offset)
{
    BYTE buffer[6];
    UINT16 dataSet[3];
    UINT16 dataReset[3];

    mmc5983maSetContinuousModeODR(config, 0);

    __mmc5983maSET(config);
    __mmc5983maWriteReg(config, INTERNAL_CONTROL_0, 0x01);
    sleepMiliseconds(10);
    __mmc5983maReadRegs(config, X_OUT_0, buffer, 6);
    dataSet[0] = (UINT16)((buffer[0] << 8) | buffer[1]);
    dataSet[1] = (UINT16)((buffer[2] << 8) | buffer[3]);
    dataSet[2] = (UINT16)((buffer[4] << 8) | buffer[5]);

    __mmc5983maRESET(config);
    __mmc5983maWriteReg(config, INTERNAL_CONTROL_0, 0x01);
    sleepMiliseconds(10);
    __mmc5983maReadRegs(config, X_OUT_0, buffer, 6);
    dataReset[0] = (UINT16)((buffer[0] << 8) | buffer[1]);
    dataReset[1] = (UINT16)((buffer[2] << 8) | buffer[3]);
    dataReset[2] = (UINT16)((buffer[4] << 8) | buffer[5]);

    offset->x = (FLOAT)(dataSet[0] + dataReset[0]) / 2.0f;
    offset->y = (FLOAT)(dataSet[1] + dataReset[1]) / 2.0f;
    offset->z = (FLOAT)(dataSet[2] + dataReset[2]) / 2.0f;

    return SUC_OK;
}

FUNCRESULT mmc5983maRead(MMC5983MAConfig *config, vec3 *mag)
{
    BYTE buffer[7];

    __mmc5983maReadRegs(config, X_OUT_0, buffer, 7);

    INT32 x = (UINT32)((buffer[0] << 10) | (buffer[1] << 2) | ((buffer[7] & 0xC0) >> 6)) - BASE_OFFSET;
    INT32 y = (UINT32)((buffer[2] << 10) | (buffer[3] << 2) | ((buffer[7] & 0x30) >> 4)) - BASE_OFFSET;
    INT32 z = (UINT32)((buffer[4] << 10) | (buffer[5] << 2) | ((buffer[7] & 0x0C) >> 2)) - BASE_OFFSET;

    FLOAT rawX = (FLOAT)x * RESOLUTION - HARD_IRON_CALIB[0];
    FLOAT rawY = (FLOAT)y * RESOLUTION - HARD_IRON_CALIB[1];
    FLOAT rawZ = (FLOAT)z * RESOLUTION - HARD_IRON_CALIB[2];

    mag->x = SOFT_IRON_CALIB[0][0] * rawX + SOFT_IRON_CALIB[0][1] * rawY + SOFT_IRON_CALIB[0][2] * rawZ;
    mag->y = SOFT_IRON_CALIB[1][0] * rawX + SOFT_IRON_CALIB[1][1] * rawY + SOFT_IRON_CALIB[1][2] * rawZ;
    mag->z = SOFT_IRON_CALIB[2][0] * rawX + SOFT_IRON_CALIB[2][1] * rawY + SOFT_IRON_CALIB[2][2] * rawZ;

    return SUC_OK;
}

FUNCRESULT mmc5983maReadTemp(MMC5983MAConfig *config, FLOAT *temp)
{
    BYTE rawTemp = __mmc5983maReadReg(config, T_OUT);

    *temp = 0.8f * (FLOAT)rawTemp - 75.0f;

    return SUC_OK;
}

VOID __mmc5983maSET(MMC5983MAConfig *config)
{
    __mmc5983maWriteReg(config, INTERNAL_CONTROL_0, 0x08);

    sleepMiliseconds(1);
}

VOID __mmc5983maRESET(MMC5983MAConfig *config)
{
    __mmc5983maWriteReg(config, INTERNAL_CONTROL_0, 0x10);

    sleepMiliseconds(1);
}

BYTE __mmc5983maReadReg(MMC5983MAConfig *config, BYTE address)
{
    BYTE data;

    address |= 0x80;

    gpioSetPinState(config->cs, GPIO_LOW);

    spiWriteBlocking(config->spi, &address, 1);
    spiReadBlocking(config->spi, 0, &data, 1);

    gpioSetPinState(config->cs, GPIO_HIGH);

    return data;
}

VOID __mmc5983maReadRegs(MMC5983MAConfig *config, BYTE address, BYTE *buffer, SIZE count)
{
    address |= 0x80;

    gpioSetPinState(config->cs, GPIO_LOW);

    spiWriteBlocking(config->spi, &address, 1);
    spiReadBlocking(config->spi, 0, buffer, count);

    gpioSetPinState(config->cs, GPIO_HIGH);
}

VOID __mmc5983maWriteReg(MMC5983MAConfig *config, BYTE address, BYTE data)
{
    address &= 0x7F;

    gpioSetPinState(config->cs, GPIO_LOW);

    spiWriteBlocking(config->spi, &address, 1);
    spiWriteBlocking(config->spi, &data, 1);

    gpioSetPinState(config->cs, GPIO_HIGH);
}