#pragma once

#define LOG_FORMAT "[%s %s]: %s\n"
#define LOG_FILENAME "log.txt"

typedef void (*logCallback_t)(const char *text);

void myLogSetCallback(logCallback_t callback);
void myLogInfo(const char *format, ...);
void myLogError(const char *format, ...);