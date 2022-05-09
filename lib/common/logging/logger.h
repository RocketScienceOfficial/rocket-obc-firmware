#pragma once

#include <stddef.h>
#include <stdarg.h>

#define MAX_LOGGER_SINKS 6

#define LOG_LEVEL_INFO "INFO"
#define LOG_LEVEL_DEBUG "DEBUG"
#define LOG_LEVEL_ERROR "ERROR"

typedef void (*logCallback_t)(const char *level, const char *msg);

typedef struct log_sink_data
{
    logCallback_t callback;
    const char *pattern;
} log_sink_data_t;

typedef struct logger_data
{
    log_sink_data_t sinks[MAX_LOGGER_SINKS];
    size_t numSinks;
} logger_data_t;

void myLogCreateLogger(logger_data_t *logger);
void myLogCreateSink(logger_data_t *logger, logCallback_t callback, const char *pattern);
char *parseLog(const char *pattern, const char *level, const char *format, va_list args);
void myLog(logger_data_t *logger, const char *level, const char *format, ...);
void myLogInfo(logger_data_t *logger, const char *format, ...);
void myLogDebug(logger_data_t *logger, const char *format, ...);
void myLogError(logger_data_t *logger, const char *format, ...);
logger_data_t *myLogGetCoreLogger();
logger_data_t *myLogGetMeasureLogger();

#define MY_LOG_CORE_PATTERN "[%l %M:%S:%m] %c\n"
#define MY_LOG_CORE_INFO(msg, ...) myLogInfo(myLogGetCoreLogger(), msg, ##__VA_ARGS__)
#define MY_LOG_CORE_DEBUG(msg, ...) myLogDebug(myLogGetCoreLogger(), msg, ##__VA_ARGS__)
#define MY_LOG_CORE_ERROR(msg, ...) myLogError(myLogGetCoreLogger(), msg, ##__VA_ARGS__)

#define MY_LOG_MEASURE_PATTERN "%c;"
#define MY_LOG_MEASURE_INFO_LEVEL "INFO"
#define MY_LOG_MEASURE_END_LEVEL "END"
#define MY_LOG_MEASURE(msg, ...) myLog(myLogGetMeasureLogger(), MY_LOG_MEASURE_INFO_LEVEL, msg, ##__VA_ARGS__)
#define MY_LOG_MEASURE_END() myLog(myLogGetMeasureLogger(), MY_LOG_MEASURE_END_LEVEL, "\n")