#pragma once

#include <obc/api.h>
#include "drivers/gpio/spi_driver.h"
#include "drivers/gpio/i2c_driver.h"
#include "drivers/gpio/gpio_driver.h"

#define NMEA_SENTENCE_MAX_LENGTH 83

/**
 * @brief The configuration of the u-blox SAM-M10Q GPS module.
 */
typedef struct UBloxSamM10QConfig
{
    BOOL useI2C;     /** Use I2C */
    SPIInstance spi; /** SPI */
    PinNumber cs;    /** CS */
    I2CInstance i2c; /** I2C */
} UBloxSamM10QConfig;

/**
 * @brief The data of the u-blox SAM-M10Q GPS module.
 */
typedef struct UBloxSamM10QData
{
    BOOL isValid;                            /** Is current data valid */
    CHAR sentence[NMEA_SENTENCE_MAX_LENGTH]; /** Sentence buffer */
    SIZE currentIndex;                       /** Current index of buffer */
    BOOL isFinishedSentence;                 /** Is current sentence ready */
} UBloxSamM10QData;

/**
 * @brief Initializes the u-blox SAM-M10Q GPS module with SPI.
 *
 * @param config The configuration of the u-blox SAM-M10Q GPS module.
 * @param spi The SPI instance to use.
 * @param miso The MISO pin to use.
 * @param mosi The MOSI pin to use.
 * @param cs The CS pin to use.
 * @param sck The SCK pin to use.
 * @return Result code
 */
FUNCRESULT uBloxSamM10QInitSPI(UBloxSamM10QConfig *config, SPIInstance spi, PinNumber miso, PinNumber mosi, PinNumber cs, PinNumber sck);

/**
 * @brief Initializes the u-blox SAM-M10Q GPS module with I2C.
 *
 * @param config The configuration of the u-blox SAM-M10Q GPS module.
 * @param i2c The I2C instance to use.
 * @param sda The SDA pin to use.
 * @param scl The SCL pin to use.
 * @return Result code
 */
FUNCRESULT uBloxSamM10QInitI2C(UBloxSamM10QConfig *config, I2CInstance i2c, PinNumber sda, PinNumber scl);

/**
 * @brief Reads a NMEA sentence from the u-blox SAM-M10Q GPS module.
 *
 * @param config The configuration of the u-blox SAM-M10Q GPS module.
 * @param data The data of the u-blox SAM-M10Q GPS module.
 * @return Result code
 */
FUNCRESULT uBloxSamM10QReadData(UBloxSamM10QConfig *config, UBloxSamM10QData *data);

/**
 * @brief Reads a byte from the u-blox SAM-M10Q GPS module.
 *
 * @param config The configuration of the u-blox SAM-M10Q GPS module.
 * @return The byte read.
 */
BYTE __uBloxSamM10QRead(UBloxSamM10QConfig *config);

/**
 * @brief Writes a byte to the u-blox SAM-M10Q GPS module.
 *
 * @param config The configuration of the u-blox SAM-M10Q GPS module.
 * @param data The byte to write.
 */
VOID __uBloxSamM10QWrite(UBloxSamM10QConfig *config, BYTE data);