#pragma once

#include "logger.h"

#define LOG_SERIAL_JSON_PATTERN "DATA:{\"type\":\"%t\",\"minutes\":%M,\"seconds\":%S,\"miliseconds\":%m,\"level\":\"%l\",\"message\":\"%c\"}\n"

void attachSerialToLog(logger_data_t* logger);