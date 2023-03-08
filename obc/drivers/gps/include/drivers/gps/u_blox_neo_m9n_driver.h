#pragma once

/**
 * REF: https://github.com/bolderflight/ublox
*/

#include <obc/api.h>
#include "drivers/gpio/spi_driver.h"
#include "drivers/gpio/gpio_driver.h"

typedef struct UBloxNeoM9NConfig
{
    SPIInstance spi;
    PinNumber cs;
} UBloxNeoM9NConfig;

FUNCRESULT uBloxNeoM9NInit(UBloxNeoM9NConfig *config, SPIInstance spi, PinNumber miso, PinNumber mosi, PinNumber cs, PinNumber sck);
BYTE __uBloxNeoM9NRead(UBloxNeoM9NConfig *config);
VOID __uBloxNeoM9NWrite(UBloxNeoM9NConfig *config, BYTE data);