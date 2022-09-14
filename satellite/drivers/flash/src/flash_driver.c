#include "drivers/flash/flash_driver.h"
#include "pico/stdlib.h"
#include "hardware/flash.h"
#include "hardware/sync.h"
#include <string.h>
#include <stdlib.h>

#define FLASH_WRITE_BUFFER_SIZE FLASH_PAGE_SIZE
#define FLASH_BASE_OFFSET (FLASH_SECTOR_SIZE * (1 << 6u))
#define FLASH_ROOT_BLOCK_SIZE FLASH_SECTOR_SIZE
#define FLASH_FILE_SIZE (FLASH_SECTOR_SIZE * (1 << 6u))
#define FLASH_UNINITIALIZED_STATE 0xffffffff
#define FLASH_INITIALIZED_STATE 0x01

static const uint8_t *readFlash(FlashModule *flashModule, uint32_t offset)
{
    return (const uint8_t *)(XIP_BASE + FLASH_BASE_OFFSET + offset);
}

static void writeFlashPage(FlashModule *flashModule, uint32_t offset, const uint8_t *buffer)
{
    uint32_t ints = save_and_disable_interrupts();
    flash_range_program(FLASH_BASE_OFFSET + offset, buffer, FLASH_WRITE_BUFFER_SIZE);
    restore_interrupts(ints);
}

static void eraseFlashSectors(FlashModule *flashModule, uint32_t offset, size_t size)
{
    uint32_t ints = save_and_disable_interrupts();
    flash_range_erase(FLASH_BASE_OFFSET + offset, size);
    restore_interrupts(ints);
}

static uint32_t getFlashFileAddressOffset(size_t index)
{
    return FLASH_ROOT_BLOCK_SIZE + (FLASH_FILE_SIZE * index);
}

static void updateFlashFilesSizes(FlashModule *module)
{
    eraseFlashSectors(module, 0, FLASH_ROOT_BLOCK_SIZE);

    uint8_t *buffer = (uint8_t *)calloc(FLASH_WRITE_BUFFER_SIZE, sizeof(uint8_t));

    memcpy(buffer, &module->_fileSystem, sizeof(module->_fileSystem));

    writeFlashPage(module, 0, buffer);

    free(buffer);
}

static void writeFlashBuffer(FlashModule *module, size_t fileIndex, const uint8_t *data, size_t size)
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

    const uint8_t *filesData = readFlash(flashModule, 0);

    memcpy(&flashModule->_fileSystem, filesData, sizeof(flashModule->_fileSystem));

    if (flashModule->_fileSystem._initializationState == FLASH_UNINITIALIZED_STATE)
    {
        flashModule->_fileSystem._initializationState = FLASH_INITIALIZED_STATE;

        memset(flashModule->_fileSystem._files, 0, sizeof(flashModule->_fileSystem._files));
        updateFlashFilesSizes(flashModule);
    }

    for (size_t i = 0; i < FLASH_FILES_COUNT; i++)
    {
        flashModule->_filesBuffers[i]._bufferSize = 0;
        flashModule->_filesBuffers[i]._buffer = (uint8_t *)calloc(FLASH_WRITE_BUFFER_SIZE, sizeof(uint8_t));
    }

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

FUNCRESULT flashWriteFile(FlashModule *module, size_t fileIndex, const char *msg)
{
    return flashWriteFileBuff(module, fileIndex, (const uint8_t *)msg, strlen(msg));
}

FUNCRESULT flashWriteFileBuff(FlashModule *module, size_t fileIndex, const uint8_t *buffer, size_t size)
{
    if (!module || fileIndex >= FLASH_FILE_SIZE || !buffer || size == 0)
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

    if (size + fileBuffer->_bufferSize > FLASH_WRITE_BUFFER_SIZE)
    {
        size_t buffOffset = FLASH_WRITE_BUFFER_SIZE - fileBuffer->_bufferSize;

        memcpy(fileBuffer->_buffer + fileBuffer->_bufferSize, buffer, buffOffset);
        fileBuffer->_bufferSize += buffOffset;

        updateFlashFilesSizes(module);
        writeFlashPage(module, getFlashFileAddressOffset(fileIndex) + fileInfo->_size, fileBuffer->_buffer);

        memset(fileBuffer->_buffer, 0, FLASH_WRITE_BUFFER_SIZE);
        fileBuffer->_bufferSize = 0;
        fileInfo->_size += FLASH_WRITE_BUFFER_SIZE;

        size_t newSize = size - buffOffset;

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

FUNCRESULT flashClearFile(FlashModule *module, size_t fileIndex)
{
    if (!module || fileIndex >= FLASH_FILE_SIZE)
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

    eraseFlashSectors(module, getFlashFileAddressOffset(fileIndex), FLASH_FILE_SIZE);

    fileInfo->_size = 0;

    updateFlashFilesSizes(module);

    return SUC_OK;
}

FUNCRESULT flashGetFile(FlashModule *module, size_t fileIndex, const uint8_t **buffer_ptr, size_t *size)
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

    *buffer_ptr = readFlash(module, getFlashFileAddressOffset(fileIndex));
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

    for (size_t i = 0; i < FLASH_FILES_COUNT; i++)
    {
        if (module->_filesBuffers[i]._bufferSize > 0)
        {
            writeFlashPage(module, getFlashFileAddressOffset(i) + module->_fileSystem._files[i]._size, module->_filesBuffers[i]._buffer);

            module->_fileSystem._files[i]._size += module->_filesBuffers[i]._bufferSize;
            module->_filesBuffers[i]._bufferSize = 0;
        }

        free(module->_filesBuffers[i]._buffer);
    }

    updateFlashFilesSizes(module);

    return SUC_OK;
}