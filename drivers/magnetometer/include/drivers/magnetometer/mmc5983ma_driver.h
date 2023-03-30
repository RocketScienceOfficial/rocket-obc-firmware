#pragma once

#include <obc/api.h>
#include "drivers/gpio/spi_driver.h"
#include "drivers/gpio/gpio_driver.h"
#include "maths/vector.h"

typedef struct MMC5983MAConfig
{
    SPIInstance spi;
    PinNumber cs;
} MMC5983MAConfig;

typedef struct MMC5983MAData
{
    vec3 mag;
} MMC5983MAData;

typedef enum MMC5983MABandwidth
{
    MMC5983MA_BW_100HZ = 0x00,
    MMC5983MA_BW_200HZ = 0x01,
    MMC5983MA_BW_400HZ = 0x02,
    MMC5983MA_BW_800HZ = 0x03,
} MMC5983MABandwidth;

typedef enum MMC5983ODR
{
    MMC5983MA_ODR_1HZ = 0x01,
    MMC5983MA_ODR_10HZ = 0x02,
    MMC5983MA_ODR_20HZ = 0x03,
    MMC5983MA_ODR_50HZ = 0x04,
    MMC5983MA_ODR_100HZ = 0x05,
    MMC5983MA_ODR_200HZ = 0x06,
    MMC5983MA_ODR_1000HZ = 0x07,
} MMC5983ODR;

FUNCRESULT mmc5983maInit(MMC5983MAConfig *config, SPIInstance spi, PinNumber miso, PinNumber mosi, PinNumber cs, PinNumber sck);
FUNCRESULT mmc5983maReset(MMC5983MAConfig *config);
FUNCRESULT smmc5983maCalculateOffset(MMC5983MAConfig *config, vec3* offset);
FUNCRESULT mmc5983maRead(MMC5983MAConfig *config, MMC5983MAData *data);
FUNCRESULT mmc5983maReadTemp(MMC5983MAConfig *config, FLOAT *temp);
FUNCRESULT mmc5983ReadStatus(MMC5983MAConfig *config, BOOL *otpReadDone, BOOL *measTDone, BOOL *measMDone);
FUNCRESULT mmc5983maSetBandwidth(MMC5983MAConfig *config, MMC5983MABandwidth bandwidth);
FUNCRESULT mmc5983maSetODR(MMC5983MAConfig *config, MMC5983ODR odr);
FUNCRESULT mmc5983maGetProductId(MMC5983MAConfig *config, BYTE *productId);

VOID __mmc5983maSET(MMC5983MAConfig *config);
VOID __mmc5983maRESET(MMC5983MAConfig *config);
BYTE __mmc5983maReadReg(MMC5983MAConfig *config, BYTE address);
VOID __mmc5983maReadRegs(MMC5983MAConfig *config, BYTE address, BYTE *buffer, SIZE count);
VOID __mmc5983maWriteReg(MMC5983MAConfig *config, BYTE address, BYTE data);