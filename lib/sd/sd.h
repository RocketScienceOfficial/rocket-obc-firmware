#pragma once

#define SD_FILES_MAX_COUNT 4

#define LOG_CORE_FILENAME "log.txt"
#define LOG_MEASURE_FILENAME "measurements.csv"

#define MY_LOG_CORE_PATTERN "[%l %M:%S:%m] %c\n"
#define MY_LOG_MEASURE_PATTERN "%c;"

int sdInit();
void sdAttachToCoreLogger();
void sdAttachToMeasureLogger();
void sdInitFile(const char *file);
int sdBegin(const char *file);
int sdWrite(const char *msg, const char *file);
int sdEnd(const char *file);
int sdFlush(const char *file);
int sdTerminate();