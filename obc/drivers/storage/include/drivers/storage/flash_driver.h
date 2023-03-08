#pragma once

#include <obc/api.h>

#define FLASH_FILES_COUNT 4

/**
 * @brief Flash file structure
 */
typedef struct _FlashFile
{
    SIZE _size; /** File size */

} _FlashFile;

/**
 * @brief Flash file buffer structure
 */
typedef struct _FlashFileBuffer
{
    BYTE *_buffer;    /** Buffer to write (Flash only allows to write 256 byte pages!) */
    SIZE _bufferSize; /** Buffer size */
} _FlashFileBuffer;

/**
 * @brief Flash file system structure
 */
typedef struct _FlashFileSystem
{
    UINT32 _initializationState;          /** Is file system initialized */
    _FlashFile _files[FLASH_FILES_COUNT]; /** Flash files */
} _FlashFileSystem;

/**
 * @brief Flash structure. All changes are done internally, so keep it just for reference!
 */
typedef struct FlashModule
{
    BOOL _isInitialized;                               /** Is Flash module initialized */
    _FlashFileSystem _fileSystem;                      /** Flash files */
    _FlashFileBuffer _filesBuffers[FLASH_FILES_COUNT]; /** Flash files buffers */
} FlashModule;

/**
 * @brief Initialize flash module.
 *
 * @param flashModule Flash module to setup
 * @return Result code
 */
FUNCRESULT flashInit(FlashModule *flashModule);

/**
 * @brief Get the Default Flash Module object
 *
 * @return Pointer to default Flash Module
 */
FlashModule *flashGetDefaultModule();

/**
 * @brief Write data to flash module. Only 256 byte pages are allowed to write, so data is buffered internally.
 *
 * @param module Flash module
 * @param fileIndex Index of file
 * @param msg Text to write
 * @return Result code
 */
FUNCRESULT flashWriteFile(FlashModule *module, SIZE fileIndex, const CHAR *msg);

/**
 * @brief Write data to flash module. Only 256 byte pages are allowed to write, so data is buffered internally
 *
 * @param module Flash module
 * @param fileIndex Index of file
 * @param buffer Buffer to write
 * @param size Size of buffer
 * @return Result code
 */
FUNCRESULT flashWriteFileBuff(FlashModule *module, SIZE fileIndex, const BYTE *buffer, SIZE size);

/**
 * @brief Clear file
 *
 * @param module Flash module
 * @param fileIndex Index of file
 * @return Result code
 */
FUNCRESULT flashClearFile(FlashModule *module, SIZE fileIndex);

/**
 * @brief Read file
 *
 * @param module Flash module
 * @param fileIndex Index of file
 * @param buffer_ptr Pointer to buffer to read to
 * @param size Size of buffer
 * @return Result code
 */
FUNCRESULT flashGetFile(FlashModule *module, SIZE fileIndex, const BYTE **buffer_ptr, SIZE *size);

/**
 * @brief Terminate flash module and flush buffers. After this call, this flash module is not usable anymore.
 *
 * @param module Flash module
 * @param fileIndex Index of file
 * @return Result code
 */
FUNCRESULT flashTerminate(FlashModule *module);