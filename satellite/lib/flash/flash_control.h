#pragma once

#include "logger.h"
#include "hardware/flash.h"
#include <stdint.h>
#include <stddef.h>

#define FLASH_WRITE_BUFFER_SIZE FLASH_PAGE_SIZE
#define FLASH_FILES_MAX_COUNT 4
#define FLASH_CALLBACKS_MAX_COUNT FLASH_FILES_MAX_COUNT

void flashInitFile(const char *file);
void flashAttachToLogger(logger_data_t* logger, const char* pattern, const char* file);
void flashWriteFile(const char *file, uint8_t buffer[FLASH_WRITE_BUFFER_SIZE]);
void flashFlushFile(const char *file);
void flashGetFile(const char *file, uint8_t **buffer_ptr, size_t *size);
void flashFileTerminate(const char* file);