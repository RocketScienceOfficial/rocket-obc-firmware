#include "drivers/gps/u_blox_neo_m9n_driver.h"
#include <string.h>
#include <stdlib.h>

FUNCRESULT uBloxNeoM9NInit(UBloxNeoM9NConfig *config, SPIInstance spi, PinNumber miso, PinNumber mosi, PinNumber cs, PinNumber sck)
{
    config->_spi = spi;
    config->_cs = cs;

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
        data->_isValid = TRUE;
        data->_currentIndex = 0;
        data->isFinishedSentence = FALSE;

        memset(data->sentence, 0, NMEA_SENTENCE_MAX_LENGTH);
    }
    else if (b == '\n')
    {
        data->sentence[data->_currentIndex] = b;
        data->sentence[data->_currentIndex + 1] = '\0';
        data->_isValid = FALSE;
        data->isFinishedSentence = TRUE;
    }

    if (data->_isValid)
    {
        data->sentence[data->_currentIndex] = b;
        data->_currentIndex++;
    }

    return SUC_OK;
}

BYTE __uBloxNeoM9NRead(UBloxNeoM9NConfig *config)
{
    BYTE data;

    gpioSetPinState(config->_cs, GPIO_LOW);
    spiReadBlocking(config->_spi, 0, &data, 1);
    gpioSetPinState(config->_cs, GPIO_HIGH);

    return data;
}

VOID __uBloxNeoM9NWrite(UBloxNeoM9NConfig *config, BYTE data)
{
    gpioSetPinState(config->_cs, GPIO_LOW);
    spiWriteBlocking(config->_spi, &data, 1);
    gpioSetPinState(config->_cs, GPIO_HIGH);
}