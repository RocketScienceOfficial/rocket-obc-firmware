#pragma once

#include "logger.h"
#include "utils/time_tracker.h"
#include <stdint.h>

/**
 * @brief Starts profile. INTERNAL USE ONLY!
 *
 * @param function Function name.
 * @return Time
 */
Timer functionProfileBegin(const char *function);

/**
 * @brief Ends profile. INTERNAL USE ONLY!
 *
 * @param function Function name.
 * @param beginResult Time returned from functionProfileBegin.
 */
void functionProfileEnd(const char *function, Timer beginResult);

Logger *myLogGetRecordLogger();

#define MY_LOG_RECORD_NAME "RECORD"
#define MY_LOG_RECORD(msg, ...) myLog(myLogGetRecordLogger(), "", msg, ##__VA_ARGS__)
#define FUNCTION_PROFILE_BEGIN_INTERNAL(func) Timer __profileBeginResult = functionProfileBegin(func)
#define FUNCTION_PROFILE_END_INTERNAL(func) functionProfileEnd(func, __profileBeginResult)
#define FUNCTION_PROFILE_BEGIN() FUNCTION_PROFILE_BEGIN_INTERNAL(__FUNCTION__)
#define FUNCTION_PROFILE_END() FUNCTION_PROFILE_END_INTERNAL(__FUNCTION__)