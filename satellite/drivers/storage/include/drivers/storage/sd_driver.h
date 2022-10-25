#pragma once

#include "tools/typedefs.h"
#include "ff.h"
#include "sd_card.h"
#include "f_util.h"

#define SD_FILES_MAX_COUNT 4
#define SD_CALLBACKS_MAX_COUNT SD_FILES_MAX_COUNT

/**
 * @brief SD card file structure. (INTERNAL USE)
 */
typedef struct _SDFile
{
    /**
     * @brief File name.
     */
    STRING _name;

    /**
     * @brief File instance
     */
    FIL _file;

    /**
     * @brief Is file opened
     */
    BOOL _isOpened;
} _SDFile;

/**
 * @brief SD card structure. All changes are done internally, so keep it just for reference!
 */
typedef struct SDCard
{
    /**
     * @brief Is SD card initialized
     */
    BOOL _isInitialized;
    /**
     * @brief SD Card instance.
     */
    sd_card_t *_sd;

    /**
     * @brief SD Card files.
     */
    _SDFile _files[SD_FILES_MAX_COUNT];

    /**
     * @brief SD Card files count.
     */
    SIZE _filesCount;
} SDCard;

/**
 * @brief Initialize SD card.
 * @param sdCard Data to setup.
 * @return Result code.
 */
FUNCRESULT sdInit(SDCard *sdCard);

/**
 * @brief Initialize file.
 * @param sdCard SD card structure.
 * @param fileName File name.
 * @return Result code.
 */
FUNCRESULT sdInitFile(SDCard *sdCard, const STRING fileName);

/**
 * @brief Open and prepare file for writing.
 * @param sdCard SD card structure.
 * @param fileName File name.
 * @return Result code.
 */
FUNCRESULT sdBegin(SDCard *sdCard, const STRING fileName);

/**
 * @brief Write message to SD card.
 * @param sdCard SD card structure.
 * @param msg Message.
 * @param fileName File name.
 * @return Result code.
 */
FUNCRESULT sdWrite(SDCard *sdCard, const STRING msg, const STRING fileName);

/**
 * @brief Close file.
 * @param sdCard SD card structure.
 * @param fileName File name.
 * @return Result code.
 */
FUNCRESULT sdEnd(SDCard *sdCard, const STRING fileName);

/**
 * @brief Clear file.
 * @param sdCard SD card structure.
 * @param fileName File name.
 * @return Result code.
 */
FUNCRESULT sdClearFile(SDCard *sdCard, const STRING fileName);

/**
 * @brief Terminate SD. After this call, SD card is not usable.
 * @param sdCard SD card structure.
 * @return Result code.
 */
FUNCRESULT sdTerminate(SDCard *sdCard);