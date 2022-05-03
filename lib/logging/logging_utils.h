#pragma once
#include <stdarg.h>

char *vlogBase(const char *format, va_list args);
char *logBase(const char *format, ...);