#include "sd.h"
#include "logger.h"
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
} sd_file_t;

static int s_SdEnabled = false;
static sd_card_t *s_pSD;
static FRESULT s_FR;
static sd_file_t s_Files[SD_FILES_MAX_COUNT];
static size_t s_FilesCount;

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

        MY_LOG_CORE_ERROR("%s error: %s (%d)\n", funcName, FRESULT_str(s_FR), s_FR);

        return 1;
    }

    return 0;
}

static FIL *getFileByName(const char *name)
{
    for (size_t i = 0; i < s_FilesCount; ++i)
    {
        if (strcmp(name, s_Files[i].name) == 0)
        {
            return &s_Files[i].file;
        }
    }

    return NULL;
}

static void __logCallback(const char *level, const char *msg)
{
    if (s_SdEnabled)
    {
        sdWrite(msg, LOG_CORE_FILENAME);
    }
}

static void __logMeasureCallback(const char *level, const char *msg)
{
    if (s_SdEnabled)
    {
        if (level == MY_LOG_MEASURE_INFO_LEVEL)
        {
            sdWrite(msg, LOG_MEASURE_FILENAME);
        }
        else if (level == MY_LOG_MEASURE_END_LEVEL)
        {
            sdWrite("\n", LOG_MEASURE_FILENAME);
        }
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

void sdAttachToCoreLogger()
{
    myLogCreateSink(myLogGetCoreLogger(), &__logCallback, MY_LOG_CORE_PATTERN);
}

void sdAttachToMeasureLogger()
{
    myLogCreateSink(myLogGetMeasureLogger(), &__logMeasureCallback, MY_LOG_MEASURE_PATTERN);
}

void sdInitFile(const char *file)
{
    sd_file_t f = {.name = file};

    s_Files[s_FilesCount] = f;
    s_FilesCount++;
}

int sdBegin(const char *file)
{
    if (!s_SdEnabled)
    {
        return 0;
    }

    FIL *f = getFileByName(file);

    if (__checkFile(f, file))
    {
        return 0;
    }

    s_FR = f_open(f, file, FA_OPEN_APPEND | FA_WRITE | FA_READ);

    if (FR_OK != s_FR && FR_EXIST != s_FR)
    {
        s_SdEnabled = false;

        MY_LOG_CORE_ERROR("f_open error: %s (%d)", FRESULT_str(s_FR), s_FR);

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

    FIL *f = getFileByName(file);

    if (__checkFile(f, file))
    {
        return 0;
    }

    int ret = f_printf(f, msg);

    if (ret < 0)
    {
        s_SdEnabled = false;

        MY_LOG_CORE_ERROR("f_printf failed (%d)", ret);

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

    FIL *f = getFileByName(file);

    if (__checkFile(f, file))
    {
        return 0;
    }

    s_FR = f_close(f);

    if (__checkError("f_close"))
    {
        return 0;
    }

    return 1;
}

int sdFlush(const char *file)
{
    if (!s_SdEnabled)
    {
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