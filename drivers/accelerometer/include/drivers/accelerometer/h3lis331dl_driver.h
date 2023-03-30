#pragma once

#include <obc/api.h>
#include "drivers/gpio/spi_driver.h"
#include "drivers/gpio/gpio_driver.h"
#include "maths/vector.h"

typedef struct H3lis331dlConfig
{
    SPIInstance spi;   /** SPI */
    PinNumber cs;      /** CS */
    FLOAT rangeFactor; /** Range factor */
} H3lis331dlConfig;

typedef struct H3lis331dlData
{
    vec3 accel; /** Acceleration in meters per second squared */
} H3lis331dlData;

typedef enum H3lis331dlODR
{
    H3LIS331DL_ODR_50HZ = 0x00,
    H3LIS331DL_ODR_100HZ = 0x01,
    H3LIS331DL_ODR_400HZ = 0x02,
    H3LIS331DL_ODR_1000HZ = 0x03,
} H3lis331dlODR;

typedef enum H3lis331dlRange
{
    H3LIS331DL_RANGE_100G = 0x00,
    H3LIS331DL_RANGE_200G = 0x01,
    H3LIS331DL_RANGE_400G = 0x03,
} H3lis331dlRange;

FUNCRESULT h3lis331dlInit(H3lis331dlConfig *config, SPIInstance spi, PinNumber miso, PinNumber mosi, PinNumber cs, PinNumber sck);
FUNCRESULT h3lis331dlWhoAmI(H3lis331dlConfig *config, BYTE *whoami);
FUNCRESULT h3lis331dlSetPower(H3lis331dlConfig *config, BOOL power);
FUNCRESULT h3lis331dlSetODR(H3lis331dlConfig *config, H3lis331dlODR odr);
FUNCRESULT h3lis331dlSetRange(H3lis331dlConfig *config, H3lis331dlRange range);
FUNCRESULT h3lis33dlStatus(H3lis331dlConfig *config, BYTE *status);
FUNCRESULT h3lis331dlRead(H3lis331dlConfig *config, H3lis331dlData *data);
BYTE __h3lis331dlReadReg(H3lis331dlConfig *config, BYTE address);
VOID __h3lis331dlReadRegs(H3lis331dlConfig *config, BYTE address, BYTE *buffer, SIZE count);
VOID __h3lis331dlWriteReg(H3lis331dlConfig *config, BYTE address, BYTE data);