#pragma once

#include "my_event.h"

#define LOG_FORMAT "[%s %s]: %s\n"
#define LOG_FILENAME "log.txt"

void myLogInit();
void myLogAddCallback(eventCallback_t callback);
void myLogInfo(const char *format, ...);
void myLogError(const char *format, ...);