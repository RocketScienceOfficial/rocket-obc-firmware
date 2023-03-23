#include "drivers/gps/u_blox_neo_m9n_driver.h"
#include <string.h>
#include <stdlib.h>

#define NMEA_SENTENCE_START '$'
#define NMEA_SENTENCE_END_0 '\r'
#define NMEA_SENTENCE_END_1 '\n'

FUNCRESULT uBloxNeoM9NInit(UBloxNeoM9NConfig *config, SPIInstance spi, PinNumber miso, PinNumber mosi, PinNumber cs, PinNumber sck)
{
    config->spi = spi;
    config->cs = cs;

    spiInitPins(spi, miso, mosi, sck, cs);

    return SUC_OK;
}

FUNCRESULT uBloxNeoM9NRead(UBloxNeoM9NConfig *config, UBloxNeoM9NData *data)
{
    BYTE b = __uBloxNeoM9NRead(config);

    if (b == NMEA_SENTENCE_START)
    {
        data->isValid = TRUE;
        data->currentIndex = 0;
        data->isFinishedSentence = FALSE;

        memset(data->sentence, 0, NMEA_SENTENCE_MAX_LENGTH);
    }
    else if (b == NMEA_SENTENCE_END_0 || b == NMEA_SENTENCE_END_1)
    {
        data->sentence[data->currentIndex] = '\0';
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