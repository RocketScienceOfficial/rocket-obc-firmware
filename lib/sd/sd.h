#pragma once

#define SD_FILES_MAX_COUNT 4

#define LOG_CORE_FILENAME "log.txt"
#define LOG_MEASUREMENTS_FILENAME "measurements.csv"

void sdInit();
void sdAttachToCoreLogger();
void sdAttachToMeasureLogger();
void sdInitFile(const char *file);
void sdBegin(const char *file);
void sdWrite(const char *msg, const char *file);
void sdEnd(const char *file);
void sdFlush(const char *file);
void sdTerminate();