#include "sd.h"
#include "time_tracker.h"
#include "sd_card.h"
#include "ff.h"
#include "rtc.h"
#include "f_util.h"
#include "hw_config.h"
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>

typedef struct sd_file
{
    const char *name;
    FIL file;
    int isOpened;
} sd_file_t;

typedef struct sd_callback_data
{
    const logger_data_t *logger;
    const sd_file_t *file;
} sd_callback_data_t;

static int s_SdEnabled = false;
static sd_card_t *s_pSD;
static FRESULT s_FR;
static sd_file_t s_Files[SD_FILES_MAX_COUNT];
static size_t s_FilesCount;
static sd_callback_data_t s_Callbacks[SD_CALLBACKS_MAX_COUNT];
static size_t s_CallbacksCount;

static int __checkFile(FIL *f, const char *file)
{
    if (!f)
    {
        s_SdEnabled = false;

        MY_LOG_CORE_ERROR("Couldn't find file: '%s'", file);

        return 1;
    }

    return 0;
}

static int __checkError(const char *funcName)
{
    if (FR_OK != s_FR)
    {
        s_SdEnabled = false;

        MY_LOG_CORE_ERROR("%s error: %s (%d)", funcName, FRESULT_str(s_FR), s_FR);

        return 1;
    }

    return 0;
}

static sd_file_t *__getSDFileByName(const char *name)
{
    for (size_t i = 0; i < s_FilesCount; ++i)
    {
        if (strcmp(name, s_Files[i].name) == 0)
        {
            return &s_Files[i];
        }
    }

    s_SdEnabled = false;

    MY_LOG_CORE_ERROR("Couldn't find file: '%s'", name);

    return NULL;
}

static const char *__getCallbackFileNameByLogger(logger_data_t *logger)
{
    for (size_t i = 0; i < s_CallbacksCount; ++i)
    {
        if (strcmp(logger->name, s_Callbacks[i].logger->name) == 0)
        {
            return s_Callbacks[i].file->name;
        }
    }

    s_SdEnabled = false;

    MY_LOG_CORE_ERROR("Couldn't find callback file for logger: '%s'", logger->name);

    return NULL;
}

static void __logCallback(logger_data_t *logger, const char *level, const char *msg)
{
    if (s_SdEnabled)
    {
        sdWrite(msg, __getCallbackFileNameByLogger(logger));
    }
}

int sdInit()
{
    MY_LOG_CORE_INFO("Initializing SD Card...");

    s_pSD = sd_get_by_num(0);

    if (!s_pSD)
    {
        s_SdEnabled = false;

        MY_LOG_CORE_ERROR("SD Card not found!");

        return 0;
    }

    s_FR = f_mount(&s_pSD->fatfs, s_pSD->pcName, 1);

    if (__checkError("f_mount"))
    {
        return 0;
    }

    s_SdEnabled = true;

    MY_LOG_CORE_INFO("SD Card initialized successfully!");

    return 1;
}

void sdAttachToLogger(logger_data_t *logger, const char *pattern, const char *file)
{
    sd_callback_data_t c = {.logger = logger, .file = __getSDFileByName(file)};

    s_Callbacks[s_CallbacksCount] = c;
    s_CallbacksCount++;

    myLogCreateSink(logger, &__logCallback, pattern);
}

void sdInitFile(const char *file)
{
    sd_file_t f = {.name = file, .isOpened = 0};

    s_Files[s_FilesCount] = f;
    s_FilesCount++;
}

int sdBegin(const char *file)
{
    if (!s_SdEnabled)
    {
        return 0;
    }

    sd_file_t *sdFile = __getSDFileByName(file);

    if (!sdFile)
    {
        MY_LOG_CORE_ERROR("File '%s' is does not exists!", file);

        return 0;
    }

    FIL *f = &sdFile->file;

    if (sdFile->isOpened)
    {
        MY_LOG_CORE_ERROR("File '%s' is already opened!", file);

        return 0;
    }

    sdFile->isOpened = true;

    if (__checkFile(f, file))
    {
        sdFile->isOpened = false;

        return 0;
    }

    s_FR = f_open(f, file, FA_OPEN_APPEND | FA_WRITE | FA_READ);

    if (FR_OK != s_FR && FR_EXIST != s_FR)
    {
        s_SdEnabled = false;

        MY_LOG_CORE_ERROR("f_open error: %s (%d)", FRESULT_str(s_FR), s_FR);

        sdFile->isOpened = false;

        return 0;
    }

    return 1;
}

int sdWrite(const char *msg, const char *file)
{
    if (!s_SdEnabled)
    {
        return 0;
    }

    sd_file_t *sdFile = __getSDFileByName(file);

    if (!sdFile)
    {
        MY_LOG_CORE_ERROR("File '%s' is does not exists!", file);

        return 0;
    }

    FIL *f = &sdFile->file;

    if (!sdFile->isOpened)
    {
        MY_LOG_CORE_ERROR("File '%s' is not opened!", file);

        return 0;
    }

    if (__checkFile(f, file))
    {
        sdFile->isOpened = false;

        return 0;
    }

    int ret = f_printf(f, msg);

    if (ret < 0)
    {
        s_SdEnabled = false;

        MY_LOG_CORE_ERROR("f_printf failed (%d)", ret);

        sdFile->isOpened = false;

        return 0;
    }

    return 1;
}

int sdEnd(const char *file)
{
    if (!s_SdEnabled)
    {
        return 0;
    }

    sd_file_t *sdFile = __getSDFileByName(file);

    if (!sdFile)
    {
        MY_LOG_CORE_ERROR("File '%s' is does not exists!", file);

        return 0;
    }

    FIL *f = &sdFile->file;

    if (!sdFile->isOpened)
    {
        MY_LOG_CORE_ERROR("File '%s' is not opened!", file);

        return 0;
    }

    if (__checkFile(f, file))
    {
        sdFile->isOpened = false;

        return 0;
    }

    s_FR = f_close(f);

    if (__checkError("f_close"))
    {
        sdFile->isOpened = false;

        return 0;
    }

    sdFile->isOpened = false;

    return 1;
}

int sdFlush(const char *file)
{
    if (!s_SdEnabled)
    {
        return 0;
    }

    sd_file_t *sdFile = __getSDFileByName(file);

    if (!sdFile)
    {
        MY_LOG_CORE_ERROR("File '%s' is does not exists!", file);

        return 0;
    }

    s_FR = f_unlink(file);

    if (s_FR == FR_NO_FILE)
    {
        return 1;
    }
    else if (__checkError("f_unlink"))
    {
        return 0;
    }

    return 1;
}

int sdTerminate()
{
    if (!s_SdEnabled)
    {
        return 0;
    }

    MY_LOG_CORE_INFO("Terminating SD Card...");

    s_FR = f_unmount(s_pSD->pcName);

    if (__checkError("f_unmount"))
    {
        return 0;
    }

    s_SdEnabled = false;

    return 1;
}