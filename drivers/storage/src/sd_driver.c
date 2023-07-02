#include "drivers/storage/sd_driver.h"
#include "hw_config.h"
#include <string.h>

static SDFile *__getSDFileByName(SDCard *sdCard, const STRING name)
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

FUNCRESULT sdInit(SDCard *sdCard, PinNumber checkPin)
{
    if (!sdCard)
    {
        return ERR_INVALIDARG;
    }

    if (sdCard->isInitialized)
    {
        return ERR_ACCESSDENIED;
    }

    BOOL connected = FALSE;

    if (FUNCFAILED(sdCheck(checkPin, &connected)))
    {
        return ERR_UNEXPECTED;
    }

    if (!connected)
    {
        return ERR_ACCESSDENIED;
    }

    memset(sdCard, 0, sizeof(SDCard));

    sdCard->sd = sd_get_by_num(0);

    if (!sdCard->sd)
    {
        return ERR_UNEXPECTED;
    }

    FRESULT fr = f_mount(&sdCard->sd->fatfs, sdCard->sd->pcName, 1);

    if (fr != FR_OK)
    {
        return ERR_FAIL;
    }

    sdCard->isInitialized = TRUE;

    return SUC_OK;
}

FUNCRESULT sdCheck(PinNumber pin, BOOL *result)
{
    GPIOState state = GPIO_LOW;

    if (FUNCFAILED(gpioGetPinState(pin, &state)))
    {
        return ERR_UNEXPECTED;
    }

    *result = state == GPIO_HIGH;

    return SUC_OK;
}

FUNCRESULT sdInitFile(SDCard *sdCard, const STRING fileName)
{
    if (!sdCard || !fileName)
    {
        return ERR_INVALIDARG;
    }

    if (!sdCard->isInitialized)
    {
        return ERR_UNINITIALIZED;
    }

    SDFile f = {
        .name = fileName,
        .isOpened = 0,
    };

    sdCard->files[sdCard->filesCount++] = f;

    return SUC_OK;
}

FUNCRESULT sdBegin(SDCard *sdCard, const STRING fileName)
{
    if (!sdCard || !fileName)
    {
        return ERR_INVALIDARG;
    }

    if (!sdCard->isInitialized)
    {
        return ERR_UNINITIALIZED;
    }

    SDFile *sdFile = __getSDFileByName(sdCard, fileName);

    if (!sdFile)
    {
        return ERR_POINTER;
    }

    FIL *f = &sdFile->file;

    if (sdFile->isOpened)
    {
        return ERR_ACCESSDENIED;
    }

    sdFile->isOpened = true;

    FRESULT fr = f_open(f, fileName, FA_OPEN_APPEND | FA_WRITE | FA_READ);

    if (fr != FR_OK && fr != FR_EXIST)
    {
        sdFile->isOpened = FALSE;

        return ERR_FAIL;
    }

    return SUC_OK;
}

FUNCRESULT sdWrite(SDCard *sdCard, const STRING msg, const STRING fileName)
{
    if (!sdCard || !msg || !fileName)
    {
        return ERR_INVALIDARG;
    }

    if (!sdCard->isInitialized)
    {
        return ERR_UNINITIALIZED;
    }

    SDFile *sdFile = __getSDFileByName(sdCard, fileName);

    if (!sdFile)
    {
        return ERR_POINTER;
    }

    FIL *f = &sdFile->file;

    if (!sdFile->isOpened)
    {
        return ERR_ACCESSDENIED;
    }

    if (!f)
    {
        sdFile->isOpened = FALSE;

        return ERR_POINTER;
    }

    INT32 ret = f_printf(f, msg);

    if (ret < 0)
    {
        sdFile->isOpened = FALSE;

        return ERR_FAIL;
    }

    return SUC_OK;
}

FUNCRESULT sdEnd(SDCard *sdCard, const STRING fileName)
{
    if (!sdCard || !fileName)
    {
        return ERR_INVALIDARG;
    }

    if (!sdCard->isInitialized)
    {
        return ERR_UNINITIALIZED;
    }

    SDFile *sdFile = __getSDFileByName(sdCard, fileName);

    if (!sdFile)
    {
        return ERR_POINTER;
    }

    FIL *f = &sdFile->file;

    if (!sdFile->isOpened)
    {
        return ERR_ACCESSDENIED;
    }

    if (!f)
    {
        sdFile->isOpened = FALSE;

        return ERR_POINTER;
    }

    FRESULT fr = f_close(f);

    if (fr != FR_OK)
    {
        sdFile->isOpened = FALSE;

        return ERR_FAIL;
    }

    sdFile->isOpened = FALSE;

    return SUC_OK;
}

FUNCRESULT sdClearFile(SDCard *sdCard, const STRING fileName)
{
    if (!sdCard || !fileName)
    {
        return ERR_INVALIDARG;
    }

    if (!sdCard->isInitialized)
    {
        return ERR_UNINITIALIZED;
    }

    SDFile *sdFile = __getSDFileByName(sdCard, fileName);

    if (!sdFile)
    {
        return ERR_POINTER;
    }

    FRESULT fr = f_unlink(fileName);

    if (fr != FR_OK && fr != FR_NO_FILE)
    {
        return ERR_FAIL;
    }

    return SUC_OK;
}

FUNCRESULT sdTerminate(SDCard *sdCard)
{
    if (!sdCard)
    {
        return ERR_INVALIDARG;
    }

    if (!sdCard->isInitialized)
    {
        return ERR_UNINITIALIZED;
    }

    FRESULT fr = f_unmount(sdCard->sd->pcName);

    if (fr != FR_OK)
    {
        return ERR_FAIL;
    }

    sdCard->isInitialized = FALSE;

    return SUC_OK;
}