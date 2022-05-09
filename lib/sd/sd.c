#include "sd.h"
#include "logger.h"
#include "time_tracker.h"
#include "sd_card.h"
#include "my_assert.h"
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

static void __measureCallback(const char *level, const char *msg)
{
    if (s_SdEnabled)
    {
        sdWrite(msg, LOG_MEASUREMENTS_FILENAME);
    }
}

void sdInit()
{
    MY_LOG_CORE_INFO("Initializing SD Card...");

    s_pSD = sd_get_by_num(0);
    s_FR = f_mount(&s_pSD->fatfs, s_pSD->pcName, 1);

    if (FR_OK != s_FR)
    {
        MY_LOG_CORE_ERROR("f_mount error: %s (%d)\n", FRESULT_str(s_FR), s_FR);

        return;
    }

    s_SdEnabled = true;

    MY_LOG_CORE_INFO("SD Card initialized successfully!");
}

void sdAttachToCoreLogger()
{
    myLogCreateSink(myLogGetCoreLogger(), &__logCallback, MY_LOG_CORE_PATTERN);
}

void sdAttachToMeasureLogger()
{
    myLogCreateSink(myLogGetMeasureLogger(), &__measureCallback, MY_LOG_MEASURE_PATTERN);
}

void sdInitFile(const char *file)
{
    sd_file_t f = {.name = file};

    s_Files[s_FilesCount] = f;
    s_FilesCount++;
}

void sdBegin(const char *file)
{
    if (!s_SdEnabled)
    {
        return;
    }

    FIL *f = getFileByName(file);

    MY_ASSERT(f != NULL);

    s_FR = f_open(f, file, FA_OPEN_APPEND | FA_WRITE);

    if (FR_OK != s_FR && FR_EXIST != s_FR)
    {
        s_SdEnabled = false;

        MY_LOG_CORE_ERROR("f_open error: %s (%d)", FRESULT_str(s_FR), s_FR);
    }
}

void sdWrite(const char *msg, const char *file)
{
    if (!s_SdEnabled)
    {
        return;
    }

    FIL *f = getFileByName(file);

    MY_ASSERT(f != NULL);

    if (f_printf(f, msg) < 0)
    {
        s_SdEnabled = false;

        MY_LOG_CORE_ERROR("f_printf failed");
    }
}

void sdEnd(const char *file)
{
    if (!s_SdEnabled)
    {
        return;
    }

    FIL *f = getFileByName(file);

    MY_ASSERT(f != NULL);

    s_FR = f_close(f);

    if (FR_OK != s_FR)
    {
        s_SdEnabled = false;

        MY_LOG_CORE_ERROR("f_close error: %s (%d)", FRESULT_str(s_FR), s_FR);
    }
}

void sdFlush(const char *file)
{
    if (!s_SdEnabled)
    {
        return;
    }

    f_unlink(file);
}

void sdTerminate()
{
    if (!s_SdEnabled)
    {
        return;
    }

    MY_LOG_CORE_INFO("Terminating SD Card...");

    f_unmount(s_pSD->pcName);

    s_SdEnabled = false;
}