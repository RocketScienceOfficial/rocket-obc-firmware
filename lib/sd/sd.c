#include <stdio.h>

#include "sd.h"
#include "logger.h"
#include "measurements_logger.h"
#include "time_tracker.h"
#include "pinout_config.h"

#include "sd_card.h"
#include "ff.h"

bool is_sd_enabled = false;
int buffer_start_time_ms = 0;
sd_entry_t *entries;
int entries_count = 0;

sd_card_t *pSD;
FRESULT fr;

static void log_callback(const char *text)
{
    if (is_sd_enabled)
    {
        sdWrite(text, LOG_FILENAME);
    }
}

static void logMeasurement_callback(const char *text)
{
    if (is_sd_enabled)
    {
        sdWrite(text, MEASURE_FILENAME);
    }
}

void sdInit()
{
    //myLogInfo("Initializing SD Card...");

    pSD = sd_get_by_num(0);
    fr = f_mount(&pSD->fatfs, pSD->pcName, 1);

    if (FR_OK != fr)
    {
        //myLogError("f_mount error: %s (%d)\n", FRESULT_str(fr), fr);

        return;
    }

    is_sd_enabled = true;

    myLogSetCallback(log_callback);
    myLogMeasureSetCallback(logMeasurement_callback);

    //myLogInfo("\n==========\n\n\tBegining of the next session!\n==========\n");

    //myLogInfo("SD Card initialized successfully!");
}

void sdWrite(const char *msg, const char *file)
{
    is_sd_enabled = true;

    int map_index = -1;

    for (int i = 0; i < entries_count; i++)
    {
        if (strcmp(entries[i].filename, file) == 0)
        {
            map_index = i;
            break;
        }
    }

    if (map_index == -1)
    {
        entries_count++;
        sd_entry_t *old_entries = entries;
        entries = (sd_entry_t *)calloc(entries_count, sizeof(sd_entry_t));

        for (int i = 0; i < entries_count - 1; i++)
        {
            entries[i] = old_entries[i];
        }

        entries[entries_count - 1].filename = file;

        free(old_entries);
    }

    char *writebuffer = entries[map_index].text;
    int len = snprintf(NULL, 0, "%s\n%s", writebuffer, msg) + 1;
    char *newformat = malloc(len);

    snprintf(newformat, len, "%s\n%s", writebuffer, msg);
    free(writebuffer);

    writebuffer = newformat;
    entries[map_index].text = writebuffer;

    if (buffer_start_time_ms - getMsSinceBoot() > SD_CARD_TIMEOUT_MS)
    {
        buffer_start_time_ms = getMsSinceBoot();

        FIL fil;

        fr = f_open(&fil, file, FA_OPEN_APPEND | FA_WRITE);

        if (FR_OK != fr && FR_EXIST != fr)
        {
            is_sd_enabled = false;

            myLogError("f_open error: %s (%d)", FRESULT_str(fr), fr);
        }

        if (f_printf(&fil, writebuffer) < 0)
        {
            is_sd_enabled = false;

            myLogError("f_printf failed");
        }

        fr = f_close(&fil);

        if (FR_OK != fr)
        {
            is_sd_enabled = false;

            myLogError("f_close error: %s (%d)", FRESULT_str(fr), fr);
        }
    }
}

void sdFlush(const char *file)
{
    f_unlink(file);
}

void sdTerminate()
{
    myLogInfo("Terminating SD Card...");

    f_unmount(pSD->pcName);

    is_sd_enabled = false;
}