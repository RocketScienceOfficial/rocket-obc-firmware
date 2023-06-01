#pragma once

#include <obc/api.h>
#include <stdarg.h>

#define MAX_LOGGER_SINKS 4
#define MAX_LOG_SIZE 256

/**
 * @brief Types of sinks
 */
typedef enum LoggerSinkType
{
    SINK_CONSOLE,
} LoggerSinkType;

/**
 * @brief Data structure for a sink (INTERNAL USE)
 */
typedef struct LogSinkData
{
    STRING pattern;      /** Pattern of sink */
    LoggerSinkType type; /** Type of sink */
    VOID *customData;    /** Custom data of sink */
} LogSinkData;

/**
 * @brief Data structure for a logger. All changes are done internally, so keep it just for reference!
 */
typedef struct Logger
{
    STRING name;                         /** Name of logger */
    LogSinkData sinks[MAX_LOGGER_SINKS]; /** Sinks of logger */
    SIZE numSinks;                       /** Number of sinks */
} Logger;

/**
 * @brief Creates a logger
 *
 * @param logger Logger data to setup
 * @param name Logger name
 */
VOID myLogCreateLogger(Logger *logger, const STRING name);

/**
 * @brief Creates a console sink
 *
 * @param logger Logger to add sink to
 * @param pattern Pattern of sink
 */
VOID myLogCreateConsoleSink(Logger *logger, const STRING pattern);

/**
 * @brief Parses a log
 *
 * @param loggerName Name of logger
 * @param pattern Pattern of log
 * @param level Level of log
 * @param format Format of log
 * @param args Arguments of log
 * @return CHAR* Parsed log
 */
STRING parseLog(const STRING loggerName, const STRING pattern, const STRING level, const STRING format, va_list args);

/**
 * @brief Logs a message
 *
 * @param loggerName Name of logger
 * @param level Level of log
 * @param format Format of log
 * @param ... Arguments of log
 */
VOID myLog(Logger *logger, const STRING level, const STRING format, ...);

/**
 * @brief Get default logger
 */
Logger *myLogGetCoreLogger();

#define MY_LOG_LEVEL_INFO "INFO"
#define MY_LOG_LEVEL_DEBUG "DEBUG"
#define MY_LOG_LEVEL_ERROR "ERROR"
#define MY_LOG_CORE_NAME "CORE_LOG"
#define MY_LOG_CORE_INFO(msg, ...) myLog(myLogGetCoreLogger(), MY_LOG_LEVEL_INFO, msg, ##__VA_ARGS__)
#define MY_LOG_CORE_DEBUG(msg, ...) myLog(myLogGetCoreLogger(), MY_LOG_LEVEL_DEBUG, msg, ##__VA_ARGS__)
#define MY_LOG_CORE_ERROR(msg, ...) myLog(myLogGetCoreLogger(), MY_LOG_LEVEL_ERROR, msg, ##__VA_ARGS__)