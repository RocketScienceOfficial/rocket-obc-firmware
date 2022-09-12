#pragma once

#include "utils/errors_types.h"
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

#define FLASH_FILES_COUNT 4

/**
 * @brief Flash file structure. (INTERNAL USE)
 */
typedef struct _FlashFile
{
    /**
     * @brief File size.
     */
    size_t _size;

} _FlashFile;

/**
 * @brief Flash file buffer structure. (INTERNAL USE)
 */
typedef struct _FlashFileBuffer
{
    /**
     * @brief Buffer to write (Flash only allows to write 256 byte pages!).
     */
    uint8_t *_buffer;

    /**
     * @brief Buffer size.
     */
    size_t _bufferSize;
} _FlashFileBuffer;

/**
 * @brief Flash file system structure. (INTERNAL USE)
 */
typedef struct _FlashFileSystem
{
    /**
     * @brief Is file system initialized.
     */
    uint32_t _initializationState;

    /**
     * @brief Flash files.
     */
    _FlashFile _files[FLASH_FILES_COUNT];
} _FlashFileSystem;

/**
 * @brief Flash structure. All changes are done internally, so keep it just for reference!
 */
typedef struct FlashModule
{
    /**
     * @brief Is Flash module initialized.
     */
    bool _isInitialized;

    /**
     * @brief Flash files.
     */
    _FlashFileSystem _fileSystem;

    /**
     * @brief Flash files buffers.
     */
    _FlashFileBuffer _filesBuffers[FLASH_FILES_COUNT];
} FlashModule;

/**
 * @brief Initialize flash module.
 *
 * @param flashModule Flash module to setup
 * @return Result code.
 */
FUNCRESULT flashInit(FlashModule *flashModule);

/**
 * @brief Get the Default Flash Module object
 *
 * @return Pointer to default Flash Module
 */
FlashModule *getDefaultFlashModule();

/**
 * @brief Write data to flash module. Only 256 byte pages are allowed to write, so data is buffered internally.
 *
 * @param module Flash module.
 * @param fileIndex Index of file.
 * @param msg Text to write.
 * @return Result code.
 */
FUNCRESULT flashWriteFile(FlashModule *module, size_t fileIndex, const char *msg);

/**
 * @brief Write data to flash module. Only 256 byte pages are allowed to write, so data is buffered internally.
 *
 * @param module Flash module.
 * @param fileIndex Index of file.
 * @param buffer Buffer to write.
 * @param size Size of buffer.
 * @return Result code.
 */
FUNCRESULT flashWriteFileBuff(FlashModule *module, size_t fileIndex, const uint8_t *buffer, size_t size);

/**
 * @brief Clear file
 *
 * @param module Flash module.
 * @param fileIndex Index of file.
 * @return Result code.
 */
FUNCRESULT flashClearFile(FlashModule *module, size_t fileIndex);

/**
 * @brief Read file.
 *
 * @param module Flash module.
 * @param fileIndex Index of file.
 * @param buffer_ptr Pointer to buffer to read to.
 * @param size Size of buffer.
 * @return Result code.
 */
FUNCRESULT flashGetFile(FlashModule *module, size_t fileIndex, const uint8_t **buffer_ptr, size_t *size);

/**
 * @brief Terminate flash module and flush buffers. After this call, this flash module is not usable anymore.
 *
 * @param module Flash module.
 * @param fileIndex Index of file
 * @return Result code.
 */
FUNCRESULT flashTerminate(FlashModule *module);