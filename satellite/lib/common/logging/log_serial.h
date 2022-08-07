#pragma once

#define DEFAULT_LOG_SERIAL_PATTERN "[%t %l %M:%S:%m] %c\n"

void logSerial(const char* format, ...);
void logSerialError(const char* format, ...);