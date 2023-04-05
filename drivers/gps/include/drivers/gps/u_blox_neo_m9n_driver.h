#pragma once

#include <obc/api.h>
#include "drivers/gpio/spi_driver.h"
#include "drivers/gpio/gpio_driver.h"

#define NMEA_SENTENCE_MAX_LENGTH 83

/**
 * @brief The configuration of the u-blox NEO-M9N GPS module.
 */
typedef struct UBloxNeoM9NConfig
{
    SPIInstance _spi; /** SPI */
    PinNumber _cs;    /** CS */
} UBloxNeoM9NConfig;

/**
 * @brief The data of the u-blox NEO-M9N GPS module.
 */
typedef struct UBloxNeoM9NData
{
    BOOL _isValid;                           /** Is current data valid */
    CHAR sentence[NMEA_SENTENCE_MAX_LENGTH]; /** Sentence buffer */
    SIZE _currentIndex;                      /** Current index of buffer */
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
FUNCRESULT uBloxNeoM9NInit(UBloxNeoM9NConfig *config, SPIInstance spi, PinNumber miso, PinNumber mosi, PinNumber cs, PinNumber sck);

/**
 * @brief Reads a NMEA sentence from the u-blox NEO-M9N GPS module.
 *
 * @param config The configuration of the u-blox NEO-M9N GPS module.
 * @param data The data of the u-blox NEO-M9N GPS module.
 * @return Result code
 */
FUNCRESULT uBloxNeoM9NReadData(UBloxNeoM9NConfig *config, UBloxNeoM9NData *data);

/**
 * @brief Reads a byte from the u-blox NEO-M9N GPS module.
 *
 * @param config The configuration of the u-blox NEO-M9N GPS module.
 * @return The byte read.
 */
BYTE __uBloxNeoM9NRead(UBloxNeoM9NConfig *config);

/**
 * @brief Writes a byte to the u-blox NEO-M9N GPS module.
 *
 * @param config The configuration of the u-blox NEO-M9N GPS module.
 * @param data The byte to write.
 */
VOID __uBloxNeoM9NWrite(UBloxNeoM9NConfig *config, BYTE data);