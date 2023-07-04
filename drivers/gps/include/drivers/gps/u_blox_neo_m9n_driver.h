#pragma once

#include <obc/api.h>
#include "nmea_parser.h"
#include "drivers/gpio/gpio_utils.h"

/**
 * @brief The configuration of the u-blox NEO-M9N GPS module.
 */
typedef struct UBloxNeoM9NConfig
{
    GPIOCommunicationConfig gpioConfig; /** The GPIO communication configuration */
} UBloxNeoM9NConfig;

/**
 * @brief The data of the u-blox NEO-M9N GPS module.
 */
typedef struct UBloxNeoM9NData
{
    BOOL isValid;                           /** Is current data valid */
    CHAR sentence[NMEA_SENTENCE_MAX_LENGTH]; /** Sentence buffer */
    SIZE currentIndex;                      /** Current index of buffer */
    BOOL isFinishedSentence;                 /** Is current sentence ready */
} UBloxNeoM9NData;

/**
 * @brief Initializes the u-blox NEO-M9N GPS module.
 *
 * @param config The configuration of the u-blox NEO-M9N GPS module.
 * @param spi The SPI instance to use.
 * @param miso The MISO pin to use.
 * @param mosi The MOSI pin to use.
 * @param cs The CS pin to use.
 * @param sck The SCK pin to use.
 * @return Result code
 */
FUNCRESULT uBloxNeoM9NInitSPI(UBloxNeoM9NConfig *config, SPIInstance spi, PinNumber miso, PinNumber mosi, PinNumber cs, PinNumber sck);

/**
 * @brief Initializes the u-blox NEO-M9N GPS module with I2C
 *
 * @param config The configuration of the u-blox NEO-M9N GPS module
 * @param i2c The I2C instance to use
 * @param sda The SDA pin to use
 * @param scl The SCL pin to use
 * @return Result code
 */
FUNCRESULT uBloxNeoM9NInitI2C(UBloxNeoM9NConfig *config, I2CInstance i2c, PinNumber sda, PinNumber scl);

/**
 * @brief Reads a NMEA sentence from the u-blox NEO-M9N GPS module.
 *
 * @param config The configuration of the u-blox NEO-M9N GPS module.
 * @param data The data of the u-blox NEO-M9N GPS module.
 * @return Result code
 */
FUNCRESULT uBloxNeoM9NReadData(UBloxNeoM9NConfig *config, UBloxNeoM9NData *data);