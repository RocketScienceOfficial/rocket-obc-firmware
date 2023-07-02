#pragma once

#include <obc/api.h>
#include "drivers/gpio/gpio_driver.h"
#include "ff.h"
#include "sd_card.h"
#include "f_util.h"

#define SD_FILES_MAX_COUNT 4

/**
 * @brief SD card file structure
 */
typedef struct SDFile
{
    STRING name;   /** File name */
    FIL file;      /** File instance */
    BOOL isOpened; /** Is file opened */
} SDFile;

/**
 * @brief SD card structure. All changes are done internally, so keep it just for reference!
 */
typedef struct SDCard
{
    BOOL isInitialized;               /** Is SD card initialized */
    sd_card_t *sd;                    /** SD Card instance */
    SDFile files[SD_FILES_MAX_COUNT]; /** SD Card files */
    SIZE filesCount;                  /** SD Card files count */
} SDCard;

/**
 * @brief Initialize SD card
 *
 * @param sdCard Data to setup
 * @param checkPin Pin to check if SD card is connected
 * @return Result code
 */
FUNCRESULT sdInit(SDCard *sdCard, PinNumber checkPin);

/**
 * @brief Check if SD card is connected
 *
 * @param pin Pin to check
 * @param result Result of check
 * @return Result code
 */
FUNCRESULT sdCheck(PinNumber pin, BOOL *result);

/**
 * @brief Initialize file
 *
 * @param sdCard SD card structure
 * @param fileName File name
 * @return Result code
 */
FUNCRESULT sdInitFile(SDCard *sdCard, const STRING fileName);

/**
 * @brief Open and prepare file for writing
 *
 * @param sdCard SD card structure
 * @param fileName File name
 * @return Result code
 */
FUNCRESULT sdBegin(SDCard *sdCard, const STRING fileName);

/**
 * @brief Write message to SD card
 *
 * @param sdCard SD card structure
 * @param msg Message
 * @param fileName File name
 * @return Result code
 */
FUNCRESULT sdWrite(SDCard *sdCard, const STRING msg, const STRING fileName);

/**
 * @brief Close file
 *
 * @param sdCard SD card structure
 * @param fileName File name
 * @return Result code
 */
FUNCRESULT sdEnd(SDCard *sdCard, const STRING fileName);

/**
 * @brief Clear file
 *
 * @param sdCard SD card structure
 * @param fileName File name
 * @return Result code
 */
FUNCRESULT sdClearFile(SDCard *sdCard, const STRING fileName);

/**
 * @brief Terminate SD. After this call, SD card is not usable.
 *
 * @param sdCard SD card structure
 * @return Result code
 */
FUNCRESULT sdTerminate(SDCard *sdCard);