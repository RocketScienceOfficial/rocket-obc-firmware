#pragma once

#include "logger.h"

unsigned int functionProfileBegin(const char* function);
void functionProfileEnd(const char* function, unsigned int beginResult);
logger_data_t *myLogGetRecordLogger();

#define MY_LOG_RECORD_NAME "RECORD"
#define MY_LOG_RECORD(msg, ...) myLog(myLogGetRecordLogger(), "", msg, ##__VA_ARGS__)
#define FUNCTION_PROFILE_BEGIN_INTERNAL(func) unsigned int profileBeginResult = functionProfileBegin(func)
#define FUNCTION_PROFILE_BEGIN() FUNCTION_PROFILE_BEGIN_INTERNAL(__FUNCTION__)
#define FUNCTION_PROFILE_END_INTERNAL(func) functionProfileEnd(func, profileBeginResult)
#define FUNCTION_PROFILE_END() FUNCTION_PROFILE_END_INTERNAL(__FUNCTION__)