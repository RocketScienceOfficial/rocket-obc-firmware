#include "sd.h"
#include "logger.h"
#include "measurements_logger.h"
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

static bool s_sdEnabled = false;
static sd_card_t *s_pSD;
static FRESULT s_fr;
static sd_file_t s_files[SD_FILES_MAX_COUNT];
static size_t s_filesCount;

static FIL *getFileByName(const char *name)
{
    for (size_t i = 0; i < s_filesCount; ++i)
    {
        if (strcmp(name, s_files[i].name) == 0)
        {
            return &s_files[i].file;
        }
    }

    return NULL;
}

static const char *__parseCallbackData(void **data)
{
    return (const char *)data[0];
}

static void log_callback(void **data, size_t size)
{
    if (s_sdEnabled)
    {
        sdWrite(__parseCallbackData(data), LOG_FILENAME);
    }
}

static void logMeasurement_callback(void **data, size_t size)
{
    if (s_sdEnabled)
    {
        sdWrite(__parseCallbackData(data), MEASURE_FILENAME);
    }
}

void sdInit()
{
    myLogInfo("Initializing SD Card...");

    s_pSD = sd_get_by_num(0);
    s_fr = f_mount(&s_pSD->fatfs, s_pSD->pcName, 1);

    if (FR_OK != s_fr)
    {
        myLogError("f_mount error: %s (%d)\n", FRESULT_str(s_fr), s_fr);

        return;
    }

    s_sdEnabled = true;

    myLogInfo("SD Card initialized successfully!");
}

void sdAttachToLogger()
{
    myLogAddCallback(&log_callback);
    myLogMeasureAddCallback(&logMeasurement_callback);
}

void sdInitFile(const char *file)
{
    sd_file_t f = {.name = file};

    s_files[s_filesCount] = f;
    s_filesCount++;
}

void sdBegin(const char *file)
{
    FIL *f = getFileByName(file);

    MY_ASSERT(f != NULL);

    s_fr = f_open(f, file, FA_OPEN_APPEND | FA_WRITE);

    if (FR_OK != s_fr && FR_EXIST != s_fr)
    {
        s_sdEnabled = false;

        myLogError("f_open error: %s (%d)", FRESULT_str(s_fr), s_fr);
    }
}

void sdWrite(const char *msg, const char *file)
{
    s_sdEnabled = true;

    FIL *f = getFileByName(file);

    MY_ASSERT(f != NULL);

    if (f_printf(f, msg) < 0)
    {
        s_sdEnabled = false;

        myLogError("f_printf failed");
    }
}

void sdEnd(const char *file)
{
    FIL *f = getFileByName(file);

    MY_ASSERT(f != NULL);

    s_fr = f_close(f);

    if (FR_OK != s_fr)
    {
        s_sdEnabled = false;

        myLogError("f_close error: %s (%d)", FRESULT_str(s_fr), s_fr);
    }
}

void sdFlush(const char *file)
{
    f_unlink(file);
}

void sdTerminate()
{
    myLogInfo("Terminating SD Card...");

    f_unmount(s_pSD->pcName);

    s_sdEnabled = false;
}