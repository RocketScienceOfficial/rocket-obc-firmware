#include "logger.h"
#include "time_tracker.h"
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>

static logCallback_t logCallback = NULL;

static void __log(const char* level, const char *format, va_list args)
{
	const char *timestamp = getTimestamp();
	int len = snprintf(NULL, 0, LOG_FORMAT, timestamp, level, format) + 1;
	char *newformat = malloc(len);

	snprintf(newformat, len, LOG_FORMAT, timestamp, level, format);

	int flen = vprintf(newformat, args) + 1;
	char buffer[128];
	vsnprintf(buffer, flen, newformat, args);

	logCallback(buffer);

	free(timestamp);
	free(newformat);
}

void myLogSetCallback(logCallback_t callback)
{
	logCallback = callback;
}

void myLogInfo(const char *format, ...)
{
	va_list ap;

	va_start(ap, format);
	__log("DEBUG", format, ap);
	va_end(ap);
}

void myLogError(const char *format, ...)
{
	va_list ap;

	va_start(ap, format);
	__log("ERROR", format, ap);
	va_end(ap);
}