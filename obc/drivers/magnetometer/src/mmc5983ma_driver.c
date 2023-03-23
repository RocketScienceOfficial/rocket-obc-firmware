#include "drivers/magnetometer/mmc5983ma_driver.h"

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

FUNCRESULT mmc5983maInit(MMC5983MAConfig *config, SPIInstance spi, PinNumber miso, PinNumber mosi, PinNumber cs, PinNumber sck)
{
    config->spi = spi;
    config->cs = cs;

    spiInitPins(spi, miso, mosi, sck, cs);

    return SUC_OK;
}

FUNCRESULT mmc5983maRead(MMC5983MAConfig *config, MMC5983MAData *mag)
{
    BYTE buffer[6];

    __mmc5983maReadRegs(config, X_OUT_0, buffer, 6);

    INT16 x = (INT16)((buffer[0] << 8) | buffer[1]);
    INT16 y = (INT16)((buffer[2] << 8) | buffer[3]);
    INT16 z = (INT16)((buffer[4] << 8) | buffer[5]);

    return SUC_OK;
}

FUNCRESULT mmc5983maReadTemp(MMC5983MAConfig *config, FLOAT *temp)
{
    BYTE rawTemp = __mmc5983maReadReg(config, T_OUT);

    *temp = 0.8f * (FLOAT)rawTemp - 75.0f;

    return SUC_OK;
}

FUNCRESULT mmc5983ReadStatus(MMC5983MAConfig *config, BOOL *otpReadDone, BOOL *measTDone, BOOL *measMDone)
{
    BYTE data = __mmc5983maReadReg(config, STATUS);

    *otpReadDone = (data & 0x10) >> 4;
    *measTDone = (data & 0x02) >> 1;
    *measMDone = (data & 0x01) >> 0;

    return SUC_OK;
}

FUNCRESULT mmc5983maSetBandwidth(MMC5983MAConfig *config, MMC5983MABandwidth bandwidth)
{
    BYTE data = __mmc5983maReadReg(config, INTERNAL_CONTROL_1);

    data &= 0xFC;
    data |= bandwidth;

    __mmc5983maWriteReg(config, INTERNAL_CONTROL_1, data);

    return SUC_OK;
}

FUNCRESULT mmc5983maReadProductId(MMC5983MAConfig *config, BYTE *productId)
{
    *productId = __mmc5983maReadReg(config, PRODUCT_ID);

    return SUC_OK;
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