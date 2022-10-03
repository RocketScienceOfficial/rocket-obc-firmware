#include "drivers/flash/flash_driver.h"
#include "pico/stdlib.h"
#include "drivers/gpio/onboard_flash_driver.h"
#include <string.h>
#include <stdlib.h>

#define FLASH_UNINITIALIZED_STATE 0xFFFFFFFF
#define FLASH_INITIALIZED_STATE 0x01

static UINT32 getFlashRootBlockSize()
{
    return flashSectorSize();
}

static UINT32 getFlashFileSize()
{
    return flashSectorSize() * (1 << 6);
}

static UINT32 getFlashFileAddressOffset(SIZE index)
{
    return getFlashRootBlockSize() + (getFlashFileSize() * index);
}

static void updateFlashFilesSizes(FlashModule *module)
{
    eraseFlashSectors(0, getFlashRootBlockSize());

    BYTE *buffer = (BYTE *)calloc(flashWriteBufferSize(), sizeof(BYTE));

    memcpy(buffer, &module->_fileSystem, sizeof(module->_fileSystem));

    writeFlashPage(0, buffer);

    free(buffer);
}

static void writeFlashBuffer(FlashModule *module, SIZE fileIndex, const BYTE *data, SIZE size)
{
    _FlashFile *fileInfo = &module->_fileSystem._files[fileIndex];
    _FlashFileBuffer *fileBuffer = &module->_filesBuffers[fileIndex];
}

FUNCRESULT flashInit(FlashModule *flashModule)
{
    if (!flashModule)
    {
        return ERR_INVALIDARG;
    }

    flashModule->_isInitialized = true;

    const BYTE *filesData;

    readFlash(0, &filesData);

    memcpy(&flashModule->_fileSystem, filesData, sizeof(flashModule->_fileSystem));

    if (flashModule->_fileSystem._initializationState == FLASH_UNINITIALIZED_STATE)
    {
        flashModule->_fileSystem._initializationState = FLASH_INITIALIZED_STATE;

        memset(flashModule->_fileSystem._files, 0, sizeof(flashModule->_fileSystem._files));
        updateFlashFilesSizes(flashModule);
    }

    for (SIZE i = 0; i < FLASH_FILES_COUNT; i++)
    {
        flashModule->_filesBuffers[i]._bufferSize = 0;
        flashModule->_filesBuffers[i]._buffer = (BYTE *)calloc(flashWriteBufferSize(), sizeof(BYTE));
    }

    return SUC_OK;
}

FlashModule *flashGetDefaultModule()
{
    static FlashModule module;
    static BOOL isInitialized = false;

    if (!isInitialized)
    {
        flashInit(&module);

        isInitialized = true;
    }

    return &module;
}

FUNCRESULT flashWriteFile(FlashModule *module, SIZE fileIndex, const CHAR *msg)
{
    return flashWriteFileBuff(module, fileIndex, (const BYTE *)msg, strlen(msg));
}

FUNCRESULT flashWriteFileBuff(FlashModule *module, SIZE fileIndex, const BYTE *buffer, SIZE size)
{
    if (!module || fileIndex >= getFlashFileSize() || !buffer || size == 0)
    {
        return ERR_INVALIDARG;
    }

    if (!module->_isInitialized)
    {
        return ERR_UNINITIALIZED;
    }

    _FlashFile *fileInfo = &module->_fileSystem._files[fileIndex];
    _FlashFileBuffer *fileBuffer = &module->_filesBuffers[fileIndex];

    if (!fileInfo || !fileBuffer)
    {
        return ERR_POINTER;
    }

    if (size + fileBuffer->_bufferSize > flashWriteBufferSize())
    {
        SIZE buffOffset = flashWriteBufferSize() - fileBuffer->_bufferSize;

        memcpy(fileBuffer->_buffer + fileBuffer->_bufferSize, buffer, buffOffset);
        fileBuffer->_bufferSize += buffOffset;

        updateFlashFilesSizes(module);
        writeFlashPage(getFlashFileAddressOffset(fileIndex) + fileInfo->_size, fileBuffer->_buffer);

        memset(fileBuffer->_buffer, 0, flashWriteBufferSize());
        fileBuffer->_bufferSize = 0;
        fileInfo->_size += flashWriteBufferSize();

        SIZE newSize = size - buffOffset;

        if (newSize > 0)
        {
            return flashWriteFileBuff(module, fileIndex, buffer + buffOffset, newSize);
        }
        else
        {
            return SUC_OK;
        }
    }
    else
    {
        memcpy(fileBuffer->_buffer + fileBuffer->_bufferSize, buffer, size);
        fileBuffer->_bufferSize += size;

        return SUC_OK;
    }
}

FUNCRESULT flashClearFile(FlashModule *module, SIZE fileIndex)
{
    if (!module || fileIndex >= getFlashFileSize())
    {
        return ERR_INVALIDARG;
    }

    if (!module->_isInitialized)
    {
        return ERR_UNINITIALIZED;
    }

    _FlashFile *fileInfo = &module->_fileSystem._files[fileIndex];

    if (!fileInfo)
    {
        return ERR_POINTER;
    }

    eraseFlashSectors(getFlashFileAddressOffset(fileIndex), getFlashFileSize());

    fileInfo->_size = 0;

    updateFlashFilesSizes(module);

    return SUC_OK;
}

FUNCRESULT flashGetFile(FlashModule *module, SIZE fileIndex, const BYTE **buffer_ptr, SIZE *size)
{
    if (!module || fileIndex >= FLASH_FILES_COUNT || !buffer_ptr || !size)
    {
        return ERR_INVALIDARG;
    }

    if (!module->_isInitialized)
    {
        return ERR_UNINITIALIZED;
    }

    _FlashFile *fileInfo = &module->_fileSystem._files[fileIndex];

    if (!fileInfo)
    {
        return ERR_POINTER;
    }

    readFlash(getFlashFileAddressOffset(fileIndex), buffer_ptr);
    *size = fileInfo->_size;

    return SUC_OK;
}

FUNCRESULT flashTerminate(FlashModule *module)
{
    if (!module)
    {
        return ERR_INVALIDARG;
    }

    if (!module->_isInitialized)
    {
        return ERR_UNINITIALIZED;
    }

    for (SIZE i = 0; i < FLASH_FILES_COUNT; i++)
    {
        if (module->_filesBuffers[i]._bufferSize > 0)
        {
            writeFlashPage(getFlashFileAddressOffset(i) + module->_fileSystem._files[i]._size, module->_filesBuffers[i]._buffer);

            module->_fileSystem._files[i]._size += module->_filesBuffers[i]._bufferSize;
            module->_filesBuffers[i]._bufferSize = 0;
        }

        free(module->_filesBuffers[i]._buffer);

        module->_filesBuffers[i]._buffer = NULL;
    }

    updateFlashFilesSizes(module);

    return SUC_OK;
}