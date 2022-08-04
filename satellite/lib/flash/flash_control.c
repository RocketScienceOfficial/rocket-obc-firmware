#include "flash_control.h"
#include "recorder.h"
#include "pico/stdlib.h"
#include "hardware/sync.h"
#include <string.h>

#define FLASH_BASE_OFFSET (256 * 1024)
#define FLASH_FILE_SIZE FLASH_SECTOR_SIZE * 64

typedef struct flash_file
{
    const char *file;
    uint32_t addressOffset;
    size_t bytes;
    uint8_t buffer[FLASH_WRITE_BUFFER_SIZE];
    size_t bufferSize;
} flash_file_t;

typedef struct flash_callback
{
    const logger_data_t *logger;
    const char *file;
} flash_callback_t;

static flash_file_t s_Files[FLASH_FILES_MAX_COUNT];
static size_t s_FilesCount;
static flash_callback_t s_Callbacks[FLASH_CALLBACKS_MAX_COUNT];
static size_t s_CallbacksCount;

static flash_file_t *__getFileByName(const char *file)
{
    for (size_t i = 0; i < s_FilesCount; i++)
    {
        if (strcmp(s_Files[i].file, file) == 0)
        {
            return &s_Files[i];
        }
    }

    MY_LOG_CORE_ERROR("Couldn't find flash file: '%s'", file);

    return NULL;
}

static const char *__getCallbackFileByLogger(logger_data_t *logger)
{
    for (size_t i = 0; i < s_CallbacksCount; i++)
    {
        if (strcmp(s_Callbacks[i].logger->name, logger->name) == 0)
        {
            return s_Callbacks[i].file;
        }
    }

    MY_LOG_CORE_ERROR("Couldn't find flash callback for logger: '%s'", logger->name);

    return NULL;
}

static void __logCallback(logger_data_t *logger, const char *level, const char *msg)
{
    const char *file = __getCallbackFileByLogger(logger);
    flash_file_t *fileInfo = __getFileByName(file);
    size_t msgSize = strlen(msg);
    size_t buffSize = fileInfo->bufferSize;

    if (msgSize + buffSize > FLASH_WRITE_BUFFER_SIZE)
    {
        size_t msgOffset = FLASH_WRITE_BUFFER_SIZE - buffSize;
        memcpy(fileInfo->buffer + buffSize, msg, msgOffset);

        flashWriteFile(file, fileInfo->buffer);

        memset(fileInfo->buffer, 0, FLASH_WRITE_BUFFER_SIZE);
        fileInfo->bufferSize = 0;

        __logCallback(logger, level, msg + msgOffset);
    }
    else
    {
        memcpy(fileInfo->buffer + buffSize, msg, msgSize);
        fileInfo->bufferSize += msgSize;
    }
}

void flashInitFile(const char *file)
{
    uint32_t addressOffset = 0;

    if (s_FilesCount > 0)
    {
        addressOffset = s_Files[s_FilesCount - 1].addressOffset + FLASH_FILE_SIZE;
    }

    flash_file_t fileInfo = {.file = file, .addressOffset = addressOffset, .bytes = 0};

    s_Files[s_FilesCount] = fileInfo;
    s_FilesCount++;
}

void flashAttachToLogger(logger_data_t *logger, const char *pattern, const char *file)
{
    flash_callback_t callback = {.logger = logger, .file = file};

    s_Callbacks[s_CallbacksCount] = callback;
    s_CallbacksCount++;

    myLogCreateSink(logger, &__logCallback, pattern);
}

void flashWriteFile(const char *file, uint8_t buffer[FLASH_WRITE_BUFFER_SIZE])
{
    flash_file_t *fileInfo = __getFileByName(file);
    uint32_t ints = save_and_disable_interrupts();
    flash_range_program(FLASH_BASE_OFFSET + fileInfo->addressOffset + fileInfo->bytes, buffer, FLASH_WRITE_BUFFER_SIZE);
    restore_interrupts(ints);

    fileInfo->bytes += FLASH_WRITE_BUFFER_SIZE;
}

void flashFlushFile(const char *file)
{
    uint32_t ints = save_and_disable_interrupts();
    flash_range_erase(FLASH_BASE_OFFSET + __getFileByName(file)->addressOffset, FLASH_FILE_SIZE);
    restore_interrupts(ints);
}

void flashGetFile(const char *file, uint8_t **buffer_ptr, size_t *size)
{
    flash_file_t *fileInfo = __getFileByName(file);

    *buffer_ptr = (uint8_t *)(XIP_BASE + FLASH_BASE_OFFSET + fileInfo->addressOffset);
    *size = fileInfo->bytes;
}

void flashFileTerminate(const char *file)
{
    flash_file_t *fileInfo = __getFileByName(file);
    flashWriteFile(file, fileInfo->buffer);
}