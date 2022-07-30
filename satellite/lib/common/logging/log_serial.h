#pragma once

#include "logger.h"

#define LOG_SERIAL_JSON_PATTERN "[%t %l %M:%S:%m] %c\n"

void attachSerialToLog(logger_data_t* logger);