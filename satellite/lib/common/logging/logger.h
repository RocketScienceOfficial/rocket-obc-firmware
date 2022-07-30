#pragma once

#include <stddef.h>
#include <stdarg.h>

#define MAX_LOGGER_SINKS 4
#define MAX_LOG_SIZE 256

typedef struct logger_data logger_data_t;
typedef void (*logCallback_t)(logger_data_t *logger, const char *level, const char *msg);

typedef struct log_sink_data
{
    logCallback_t callback;
    const char *pattern;
} log_sink_data_t;

typedef struct logger_data
{
    const char *name;
    log_sink_data_t sinks[MAX_LOGGER_SINKS];
    size_t numSinks;
} logger_data_t;

void myLogCreateLogger(logger_data_t *logger, const char *name);
void myLogCreateSink(logger_data_t *logger, logCallback_t callback, const char *pattern);
char *parseLog(const char *loggerName, const char *pattern, const char *level, const char *format, va_list args);
void myLog(logger_data_t *logger, const char *level, const char *format, ...);
logger_data_t *myLogGetCoreLogger();

#define MY_LOG_LEVEL_INFO "INFO"
#define MY_LOG_LEVEL_DEBUG "DEBUG"
#define MY_LOG_LEVEL_ERROR "ERROR"
#define MY_LOG_CORE_NAME "CORE_LOG"
#define MY_LOG_CORE_INFO(msg, ...) myLog(myLogGetCoreLogger(), MY_LOG_LEVEL_INFO, msg, ##__VA_ARGS__)
#define MY_LOG_CORE_DEBUG(msg, ...) myLog(myLogGetCoreLogger(), MY_LOG_LEVEL_DEBUG, msg, ##__VA_ARGS__)
#define MY_LOG_CORE_ERROR(msg, ...) myLog(myLogGetCoreLogger(), MY_LOG_LEVEL_ERROR, msg, ##__VA_ARGS__)