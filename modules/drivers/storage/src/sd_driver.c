#include "modules/drivers/storage/sd_driver.h"
#include "hw_config.h"
#include <string.h>

static sd_file_t *_get_sd_file_by_name(sd_card_inst_t *sdCard, const char *name)
{
    for (size_t i = 0; i < sdCard->filesCount; ++i)
    {
        if (strcmp(name, sdCard->files[i].name) == 0)
        {
            return &sdCard->files[i];
        }
    }

    return NULL;
}

void sd_init(sd_card_inst_t *sdCard, hal_pin_number_t checkPin)
{
    if (!sdCard)
    {
        return;
    }

    if (sdCard->isInitialized)
    {
        return;
    }

    bool connected = false;
    sd_check(checkPin, &connected);

    if (!connected)
    {
        return;
    }

    memset(sdCard, 0, sizeof(sd_card_inst_t));

    sdCard->sd = sd_get_by_num(0);

    if (!sdCard->sd)
    {
        return;
    }

    FRESULT fr = f_mount(&sdCard->sd->fatfs, sdCard->sd->pcName, 1);

    if (fr != FR_OK)
    {
        return;
    }

    sdCard->isInitialized = true;
}

void sd_check(hal_pin_number_t pin, bool *result)
{
    gpio_state_t state;
    hal_gpio_get_pin_state(pin, &state);

    *result = state == GPIO_HIGH;
}

void sd_init_file(sd_card_inst_t *sdCard, const char *fileName)
{
    if (!sdCard || !fileName)
    {
        return;
    }

    if (!sdCard->isInitialized)
    {
        return;
    }

    sd_file_t f = {
        .name = fileName,
        .isOpened = 0,
    };

    sdCard->files[sdCard->filesCount++] = f;
}

void sd_begin(sd_card_inst_t *sdCard, const char *fileName)
{
    if (!sdCard || !fileName)
    {
        return;
    }

    if (!sdCard->isInitialized)
    {
        return;
    }

    sd_file_t *sdFile = _get_sd_file_by_name(sdCard, fileName);

    if (!sdFile)
    {
        return;
    }

    FIL *f = &sdFile->file;

    if (sdFile->isOpened)
    {
        return;
    }

    sdFile->isOpened = true;

    FRESULT fr = f_open(f, fileName, FA_OPEN_APPEND | FA_WRITE | FA_READ);

    if (fr != FR_OK && fr != FR_EXIST)
    {
        sdFile->isOpened = false;

        return;
    }
}

void sd_write(sd_card_inst_t *sdCard, const char *msg, const char *fileName)
{
    if (!sdCard || !msg || !fileName)
    {
        return;
    }

    if (!sdCard->isInitialized)
    {
        return;
    }

    sd_file_t *sdFile = _get_sd_file_by_name(sdCard, fileName);

    if (!sdFile)
    {
        return;
    }

    FIL *f = &sdFile->file;

    if (!sdFile->isOpened)
    {
        return;
    }

    if (!f)
    {
        sdFile->isOpened = false;

        return;
    }

    int ret = f_printf(f, msg);

    if (ret < 0)
    {
        sdFile->isOpened = false;

        return;
    }
}

void sd_end(sd_card_inst_t *sdCard, const char *fileName)
{
    if (!sdCard || !fileName)
    {
        return;
    }

    if (!sdCard->isInitialized)
    {
        return;
    }

    sd_file_t *sdFile = _get_sd_file_by_name(sdCard, fileName);

    if (!sdFile)
    {
        return;
    }

    FIL *f = &sdFile->file;

    if (!sdFile->isOpened)
    {
        return;
    }

    if (!f)
    {
        sdFile->isOpened = false;

        return;
    }

    FRESULT fr = f_close(f);

    if (fr != FR_OK)
    {
        sdFile->isOpened = false;

        return;
    }

    sdFile->isOpened = false;
}

void sd_clear_file(sd_card_inst_t *sdCard, const char *fileName)
{
    if (!sdCard || !fileName)
    {
        return;
    }

    if (!sdCard->isInitialized)
    {
        return;
    }

    sd_file_t *sdFile = _get_sd_file_by_name(sdCard, fileName);

    if (!sdFile)
    {
        return;
    }

    FRESULT fr = f_unlink(fileName);

    if (fr != FR_OK && fr != FR_NO_FILE)
    {
        return;
    }
}

void sd_terminate(sd_card_inst_t *sdCard)
{
    if (!sdCard)
    {
        return;
    }

    if (!sdCard->isInitialized)
    {
        return;
    }

    FRESULT fr = f_unmount(sdCard->sd->pcName);

    if (fr != FR_OK)
    {
        return;
    }

    sdCard->isInitialized = false;
}