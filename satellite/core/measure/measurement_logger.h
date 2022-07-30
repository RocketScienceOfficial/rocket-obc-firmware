#pragma once

#include "logger.h"

logger_data_t *myLogGetMeasureLogger();

#define MY_LOG_MEASURE_NAME "MEASURE_LOG"
#define MY_LOG_MEASURE_INT(value) myLog(myLogGetMeasureLogger(), "", "%d", value)
#define MY_LOG_MEASURE_FLOAT(value) myLog(myLogGetMeasureLogger(), "", "%f", value)
#define MY_LOG_MEASURE_END() myLog(myLogGetMeasureLogger(), "", "\n")