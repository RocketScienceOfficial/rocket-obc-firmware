#include "drivers/gps/u_blox_neo_m9n_driver.h"
#include <string.h>
#include <stdlib.h>

FUNCRESULT uBloxNeoM9NInit(UBloxNeoM9NConfig *config, SPIInstance spi, PinNumber miso, PinNumber mosi, PinNumber cs, PinNumber sck)
{
    config->spi = spi;
    config->cs = cs;

    spiInitPins(spi, miso, mosi, sck, cs);

    return SUC_OK;
}

FUNCRESULT uBloxNeoM9NReadData(UBloxNeoM9NConfig *config, UBloxNeoM9NData *data)
{
    BYTE b = __uBloxNeoM9NRead(config);

    if (b == 0xFF)
    {
        return SUC_OK;
    }

    if (b == '$')
    {
        data->isValid = TRUE;
        data->currentIndex = 0;
        data->isFinishedSentence = FALSE;

        memset(data->sentence, 0, NMEA_SENTENCE_MAX_LENGTH);
    }
    else if (b == '\n')
    {
        data->sentence[data->currentIndex] = b;
        data->sentence[data->currentIndex + 1] = '\0';
        data->isValid = FALSE;
        data->isFinishedSentence = TRUE;
    }

    if (data->isValid)
    {
        data->sentence[data->currentIndex] = b;
        data->currentIndex++;
    }

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