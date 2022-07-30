#pragma once

#define LOG_CORE_FILENAME "log.txt"
#define LOG_MEASURE_FILENAME "measurements.csv"
#define LOG_RECORD_FILENAME "record.txt"

#define MY_LOG_CORE_PATTERN "[%l %M:%S:%m] %c\n"
#define MY_LOG_MEASURE_PATTERN "%c;"
#define MY_LOG_RECORD_PATTERN "[%l %M:%S:%m] %c\n"

void initializeSd();
void beginSd();
void endSd();