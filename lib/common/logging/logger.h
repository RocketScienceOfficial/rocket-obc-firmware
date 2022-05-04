#pragma once

#include "my_event.h"

#define LOG_FORMAT "[%s %s]: %s\n"
#define LOG_FILENAME "log.txt"

#define LOG_LEVEL_DEBUG "DEBUG"
#define LOG_LEVEL_ERROR "ERROR"
#define LOG_LEVEL_RAW ""
#define LOG_LEVEL_CONSOLE "CONSOLE"

void myLogInit();
void myLogAddCallback(eventCallback_t callback);
void myLogInfo(const char *format, ...);
void myLogError(const char *format, ...);
void myLogRaw(const char *format, ...);
void myLogConsole(const char *format, ...);