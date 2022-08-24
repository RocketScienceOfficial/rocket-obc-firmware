#pragma once

#include <stddef.h>
#include <stdarg.h>

#define MAX_LOGGER_SINKS 4
#define MAX_LOG_SIZE 256

/**
 * @brief Types of sinks.
 */
typedef enum LoggerSinkType
{
    SINK_CONSOLE,
    SINK_FILE,
} LoggerSinkType;

/**
 * @brief Data structure for a sink. (INTERNAL USE)
 */
typedef struct LogSinkData
{
    /**
     * @brief Pattern of sink.
     */
    const char *_pattern;

    /**
     * @brief Type of sink.
     */
    LoggerSinkType _type;

    /**
     * @brief Custom data of sink.
     */
    void *_customData;
} LogSinkData;

/**
 * @brief Data structure for a logger. All changes are done internally, so keep it just for reference!
 */
typedef struct Logger
{
    /**
     * @brief Name of logger.
     */
    const char *_name;

    /**
     * @brief Sinks of logger.
     *
     */
    LogSinkData _sinks[MAX_LOGGER_SINKS];

    /**
     * @brief Number of sinks.
     */
    size_t _numSinks;
} Logger;

/**
 * @brief Creates a logger.
 *
 * @param logger Logger data to setup.
 * @param name Logger name.
 */
void myLogCreateLogger(Logger *logger, const char *name);

/**
 * @brief Creates a console sink.
 *
 * @param logger Logger to add sink to.
 * @param pattern Pattern of sink.
 */
void myLogCreateConsoleSink(Logger *logger, const char *pattern);

/**
 * @brief Creates a file sink.
 *
 * @param logger Logger to add sink to.
 * @param pattern Pattern of sink.
 */
void myLogCreateFileSink(Logger *logger, const char *pattern, size_t fileIndex);

/**
 * @brief Parses a log.
 *
 * @param loggerName Name of logger.
 * @param pattern Pattern of log.
 * @param level Level of log.
 * @param format Format of log.
 * @param args Arguments of log.
 * @return char* Parsed log.
 */
char *parseLog(const char *loggerName, const char *pattern, const char *level, const char *format, va_list args);

/**
 * @brief Logs a message.
 *
 * @param loggerName Name of logger.
 * @param level Level of log.
 * @param format Format of log.
 * @param ... Arguments of log.
 */
void myLog(Logger *logger, const char *level, const char *format, ...);

Logger *myLogGetCoreLogger();

#define MY_LOG_LEVEL_INFO "INFO"
#define MY_LOG_LEVEL_DEBUG "DEBUG"
#define MY_LOG_LEVEL_ERROR "ERROR"
#define MY_LOG_CORE_NAME "CORE_LOG"
#define MY_LOG_CORE_INFO(msg, ...) myLog(myLogGetCoreLogger(), MY_LOG_LEVEL_INFO, msg, ##__VA_ARGS__)
#define MY_LOG_CORE_DEBUG(msg, ...) myLog(myLogGetCoreLogger(), MY_LOG_LEVEL_DEBUG, msg, ##__VA_ARGS__)
#define MY_LOG_CORE_ERROR(msg, ...) myLog(myLogGetCoreLogger(), MY_LOG_LEVEL_ERROR, msg, ##__VA_ARGS__)