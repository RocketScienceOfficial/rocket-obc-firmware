#include "drivers/gps/u_blox_neo_m9n_driver.h"
#include <string.h>
#include <stdlib.h>

#define I2C_ADDRESS 0x42

FUNCRESULT uBloxNeoM9NInitSPI(UBloxNeoM9NConfig *config, SPIInstance spi, PinNumber miso, PinNumber mosi, PinNumber cs, PinNumber sck)
{
    config->gpioConfig = (GPIOCommunicationConfig){
        .protocol = GPIO_PROTOCOL_SPI,
        .spi = spi,
        .cs = cs,
    };

    spiInitPins(spi, miso, mosi, sck, cs);

    return SUC_OK;
}

FUNCRESULT uBloxNeoM9NInitI2C(UBloxNeoM9NConfig *config, I2CInstance i2c, PinNumber sda, PinNumber scl)
{
    config->gpioConfig = (GPIOCommunicationConfig){
        .protocol = GPIO_PROTOCOL_I2C,
        .i2c = i2c,
        .i2cAddress = I2C_ADDRESS,
    };

    i2cInitPins(i2c, sda, scl);

    return SUC_OK;
}

FUNCRESULT uBloxNeoM9NReadData(UBloxNeoM9NConfig *config, UBloxNeoM9NData *data)
{
    BYTE b = gpioSingleRead(&config->gpioConfig);

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