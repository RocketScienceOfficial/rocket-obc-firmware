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

FUNCRESULT mmc5983maInit(MMC5983MAConfig *config, SPIInstance spi, PinNumber miso, PinNumber mosi, PinNumber cs, PinNumber sck);
FUNCRESULT mmc5983maRead(MMC5983MAConfig *config, MMC5983MAData *mag);
FUNCRESULT mmc5983maReadTemp(MMC5983MAConfig *config, FLOAT *temp);
FUNCRESULT mmc5983ReadStatus(MMC5983MAConfig *config, BOOL *otpReadDone, BOOL *measTDone, BOOL *measMDone);
FUNCRESULT mmc5983maSetBandwidth(MMC5983MAConfig *config, MMC5983MABandwidth bandwidth);
FUNCRESULT mmc5983maReadProductId(MMC5983MAConfig *config, BYTE *productId);
BYTE __mmc5983maReadReg(MMC5983MAConfig *config, BYTE address);
VOID __mmc5983maReadRegs(MMC5983MAConfig *config, BYTE address, BYTE *buffer, SIZE count);
VOID __mmc5983maWriteReg(MMC5983MAConfig *config, BYTE address, BYTE data);