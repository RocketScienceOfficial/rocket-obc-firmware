#pragma once

#include "logger.h"

#define SD_FILES_MAX_COUNT 4
#define SD_CALLBACKS_MAX_COUNT 4

int sdInit();
void sdAttachToLogger(logger_data_t* logger, const char* pattern, const char* file);
void sdInitFile(const char *file);
int sdBegin(const char *file);
int sdWrite(const char *msg, const char *file);
int sdEnd(const char *file);
int sdFlush(const char *file);
int sdTerminate();