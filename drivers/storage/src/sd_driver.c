#include "drivers/storage/sd_driver.h"
#include "hw_config.h"
#include <string.h>

static _SDFile *__getSDFileByName(SDCard *sdCard, const STRING name)
{
    for (size_t i = 0; i < sdCard->_filesCount; ++i)
    {
        if (strcmp(name, sdCard->_files[i]._name) == 0)
        {
            return &sdCard->_files[i];
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

    if (sdCard->_isInitialized)
    {
        return ERR_ACCESSDENIED;
    }

    BOOL connected = FALSE;

    // if (FUNCFAILED(sdCheck(checkPin, &connected)))
    // {
    //     return ERR_UNEXPECTED;
    // }

    // if (!connected)
    // {
    //     return ERR_ACCESSDENIED;
    // }

    memset(sdCard, 0, sizeof(SDCard));

    sdCard->_sd = sd_get_by_num(0);

    if (!sdCard->_sd)
    {
        return ERR_UNEXPECTED;
    }

    FRESULT fr = f_mount(&sdCard->_sd->fatfs, sdCard->_sd->pcName, 1);

    if (fr != FR_OK)
    {
        return ERR_FAIL;
    }

    sdCard->_isInitialized = TRUE;

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

    if (!sdCard->_isInitialized)
    {
        return ERR_UNINITIALIZED;
    }

    _SDFile f = {
        ._name = fileName,
        ._isOpened = 0,
    };

    sdCard->_files[sdCard->_filesCount++] = f;

    return SUC_OK;
}

FUNCRESULT sdBegin(SDCard *sdCard, const STRING fileName)
{
    if (!sdCard || !fileName)
    {
        return ERR_INVALIDARG;
    }

    if (!sdCard->_isInitialized)
    {
        return ERR_UNINITIALIZED;
    }

    _SDFile *sdFile = __getSDFileByName(sdCard, fileName);

    if (!sdFile)
    {
        return ERR_POINTER;
    }

    FIL *f = &sdFile->_file;

    if (sdFile->_isOpened)
    {
        return ERR_ACCESSDENIED;
    }

    sdFile->_isOpened = true;

    FRESULT fr = f_open(f, fileName, FA_OPEN_APPEND | FA_WRITE | FA_READ);

    if (fr != FR_OK && fr != FR_EXIST)
    {
        sdFile->_isOpened = FALSE;

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

    if (!sdCard->_isInitialized)
    {
        return ERR_UNINITIALIZED;
    }

    _SDFile *sdFile = __getSDFileByName(sdCard, fileName);

    if (!sdFile)
    {
        return ERR_POINTER;
    }

    FIL *f = &sdFile->_file;

    if (!sdFile->_isOpened)
    {
        return ERR_ACCESSDENIED;
    }

    if (!f)
    {
        sdFile->_isOpened = FALSE;

        return ERR_POINTER;
    }

    INT32 ret = f_printf(f, msg);

    if (ret < 0)
    {
        sdFile->_isOpened = FALSE;

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

    if (!sdCard->_isInitialized)
    {
        return ERR_UNINITIALIZED;
    }

    _SDFile *sdFile = __getSDFileByName(sdCard, fileName);

    if (!sdFile)
    {
        return ERR_POINTER;
    }

    FIL *f = &sdFile->_file;

    if (!sdFile->_isOpened)
    {
        return ERR_ACCESSDENIED;
    }

    if (!f)
    {
        sdFile->_isOpened = FALSE;

        return ERR_POINTER;
    }

    FRESULT fr = f_close(f);

    if (fr != FR_OK)
    {
        sdFile->_isOpened = FALSE;

        return ERR_FAIL;
    }

    sdFile->_isOpened = FALSE;

    return SUC_OK;
}

FUNCRESULT sdClearFile(SDCard *sdCard, const STRING fileName)
{
    if (!sdCard || !fileName)
    {
        return ERR_INVALIDARG;
    }

    if (!sdCard->_isInitialized)
    {
        return ERR_UNINITIALIZED;
    }

    _SDFile *sdFile = __getSDFileByName(sdCard, fileName);

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

    if (!sdCard->_isInitialized)
    {
        return ERR_UNINITIALIZED;
    }

    FRESULT fr = f_unmount(sdCard->_sd->pcName);

    if (fr != FR_OK)
    {
        return ERR_FAIL;
    }

    sdCard->_isInitialized = FALSE;

    return SUC_OK;
}