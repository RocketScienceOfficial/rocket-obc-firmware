#include "flash_control.h"
#include "pico/stdlib.h"
#include "hardware/sync.h"
#include <string.h>

static _FlashFile *__getFlashFileByName(FlashModule *module, const char *fileName)
{
    for (size_t i = 0; i < module->_filesCount; i++)
    {
        if (strcmp(module->_files[i]._name, fileName) == 0)
        {
            return &module->_files[i];
        }
    }

    return NULL;
}

FUNCRESULT flashInit(FlashModule *flashModule)
{
    if (!flashModule)
    {
        return ERR_INVALIDARG;
    }

    flashModule->_isInitialized = true;
    flashModule->_filesCount = 0;

    return SUC_OK;
}

FUNCRESULT flashInitFile(FlashModule *module, const char *fileName)
{
    if (!module || !fileName)
    {
        return ERR_INVALIDARG;
    }

    if (!module->_isInitialized)
    {
        return ERR_UNINITIALIZED;
    }

    uint32_t addressOffset = 0;

    if (module->_filesCount > 0)
    {
        addressOffset = module->_files[module->_filesCount - 1]._addressOffset + FLASH_FILE_SIZE;
    }

    _FlashFile fileInfo = {._name = fileName, ._addressOffset = addressOffset, ._bytes = 0, ._bufferSize = 0};

    module->_files[module->_filesCount++] = fileInfo;

    return SUC_OK;
}

FlashModule *getDefaultFlashModule()
{
    static FlashModule module;
    static bool isInitialized = false;

    if (!isInitialized)
    {
        flashInit(&module);

        isInitialized = true;
    }

    return &module;
}

FUNCRESULT flashWriteFile(FlashModule *module, const char *fileName, const char *msg)
{
    if (!module || !fileName || !msg)
    {
        return ERR_INVALIDARG;
    }

    if (!module->_isInitialized)
    {
        return ERR_UNINITIALIZED;
    }

    _FlashFile *fileInfo = __getFlashFileByName(module, fileName);

    if (!fileInfo)
    {
        return ERR_POINTER;
    }

    size_t msgSize = strlen(msg);
    size_t buffSize = fileInfo->_bufferSize;

    if (msgSize + buffSize > FLASH_WRITE_BUFFER_SIZE)
    {
        size_t msgOffset = FLASH_WRITE_BUFFER_SIZE - buffSize;
        memcpy(fileInfo->_buffer + buffSize, msg, msgOffset);

        uint32_t ints = save_and_disable_interrupts();
        flash_range_program(FLASH_BASE_OFFSET + fileInfo->_addressOffset + fileInfo->_bytes, msg, FLASH_WRITE_BUFFER_SIZE);
        restore_interrupts(ints);

        fileInfo->_bytes += FLASH_WRITE_BUFFER_SIZE;

        memset(fileInfo->_buffer, 0, FLASH_WRITE_BUFFER_SIZE);
        fileInfo->_bufferSize = 0;

        return flashWriteFile(module, fileName, msg + msgOffset);
    }
    else
    {
        memcpy(fileInfo->_buffer + buffSize, msg, msgSize);
        fileInfo->_bufferSize += msgSize;

        return SUC_OK;
    }
}

FUNCRESULT flashFlushFile(FlashModule *module, const char *fileName)
{
    if (!module || !fileName)
    {
        return ERR_INVALIDARG;
    }

    if (!module->_isInitialized)
    {
        return ERR_UNINITIALIZED;
    }

    _FlashFile *fileInfo = __getFlashFileByName(module, fileName);

    if (!fileInfo)
    {
        return ERR_POINTER;
    }

    uint32_t ints = save_and_disable_interrupts();
    flash_range_erase(FLASH_BASE_OFFSET + fileInfo->_addressOffset, FLASH_FILE_SIZE);
    restore_interrupts(ints);

    return SUC_OK;
}

FUNCRESULT flashGetFile(FlashModule *module, const char *fileName, uint8_t **buffer_ptr, size_t *size)
{
    if (!module || !fileName || !buffer_ptr || !size)
    {
        return ERR_INVALIDARG;
    }

    if (!module->_isInitialized)
    {
        return ERR_UNINITIALIZED;
    }

    _FlashFile *fileInfo = __getFlashFileByName(module, fileName);

    if (!fileInfo)
    {
        return ERR_POINTER;
    }

    *buffer_ptr = (uint8_t *)(XIP_BASE + FLASH_BASE_OFFSET + fileInfo->_addressOffset);
    *size = fileInfo->_bytes;

    return SUC_OK;
}

FUNCRESULT flashFileTerminate(FlashModule *module, const char *fileName)
{
    if (!module || !fileName)
    {
        return ERR_INVALIDARG;
    }

    if (!module->_isInitialized)
    {
        return ERR_UNINITIALIZED;
    }

    _FlashFile *fileInfo = __getFlashFileByName(module, fileName);

    if (!fileInfo)
    {
        return ERR_POINTER;
    }

    FUNCRESULT res = flashWriteFile(module, fileName, fileInfo->_buffer);

    module->_isInitialized = false;

    return res;
}