#pragma once

#include "logger.h"
#include "tools/typedefs.h"
#include "tools/time_tracker.h"

/**
 * @brief Starts profile. INTERNAL USE ONLY!
 *
 * @param function Function name.
 * @return Time
 */
TIME functionProfileBegin(STRING function);

/**
 * @brief Ends profile. INTERNAL USE ONLY!
 *
 * @param function Function name.
 * @param beginResult Time returned from functionProfileBegin.
 */
VOID functionProfileEnd(STRING function, TIME beginResult);

Logger *myLogGetRecordLogger();

#define MY_LOG_RECORD_NAME "RECORD"
#define MY_LOG_RECORD(msg, ...) myLog(myLogGetRecordLogger(), "", msg, ##__VA_ARGS__)
#define FUNCTION_PROFILE_BEGIN_INTERNAL(func) TIME __profileBeginResult = functionProfileBegin((STRING)func)
#define FUNCTION_PROFILE_END_INTERNAL(func) functionProfileEnd((STRING)func, __profileBeginResult)
#define FUNCTION_PROFILE_BEGIN() FUNCTION_PROFILE_BEGIN_INTERNAL(__FUNCTION__)
#define FUNCTION_PROFILE_END() FUNCTION_PROFILE_END_INTERNAL(__FUNCTION__)