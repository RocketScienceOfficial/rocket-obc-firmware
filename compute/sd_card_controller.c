#include <stdio.h>

#include "sd_card_controller.h"
#include "logger.h"
#include "pinout_config.h"

#include "sd_card.h"
#include "ff.h"

sd_card_t *pSD;
FRESULT fr;

void sd_init()
{
    my_log_notout("Initializing SD Card...");

    pSD = sd_get_by_num(0);
    fr = f_mount(&pSD->fatfs, pSD->pcName, 1);

    if (FR_OK != fr)
    {
        my_log_error_notout("f_mount error: %s (%d)\n", FRESULT_str(fr), fr);
    }

    my_log("\n==========\n\n\tBegining of the next session!\n==========\n");

    my_log("SD Card initialized successfully!");
}

void sd_write(const char *msg, const char *file)
{
    FIL fil;

    fr = f_open(&fil, file, FA_OPEN_APPEND | FA_WRITE);

    if (FR_OK != fr && FR_EXIST != fr)
    {
        my_log_error_notout("f_open error: %s (%d)", FRESULT_str(fr), fr);
    }

    if (f_printf(&fil, msg) < 0)
    {
        my_log_error_notout("f_printf failed");
    }

    fr = f_close(&fil);

    if (FR_OK != fr)
    {
        my_log_error_notout("f_close error: %s (%d)", FRESULT_str(fr), fr);
    }
}

void sd_terminate()
{
    my_log("Terminating SD Card...");

    f_unmount(pSD->pcName);
}