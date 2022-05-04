#include "logger.h"
#include "logging_utils.h"
#include "time_tracker.h"
#include <stdlib.h>
#include <stdio.h>

static event_handler_t s_eventHandler;

static void __log(const char *level, const char *format, va_list args)
{
	if (level == LOG_LEVEL_DEBUG || level == LOG_LEVEL_ERROR)
	{
		char *timestamp = getTimestamp();
		char *message = vlogBase(format, args);
		char *log = logBase(LOG_FORMAT, level, timestamp, message);
		void *data[] = {log, level};

		callEvent(&s_eventHandler, data, 2);

		free(timestamp);
		free(message);
		free(log);
	}
	else if (level == LOG_LEVEL_RAW)
	{
		char *message = logBase("%s\n", format);
		char *log = vlogBase(message, args);
		void *data[] = {log, level};

		callEvent(&s_eventHandler, data, 2);

		free(message);
		free(log);
	}
	else if (level == LOG_LEVEL_CONSOLE)
	{
		char *log = vlogBase(format, args);
		void *data[] = {log, level};

		callEvent(&s_eventHandler, data, 2);

		free(log);
	}
}

void myLogInit()
{
	initHandler(&s_eventHandler);
}

void myLogAddCallback(eventCallback_t callback)
{
	addEvent(&s_eventHandler, callback, NULL);
}

void myLogInfo(const char *format, ...)
{
	va_list ap;

	va_start(ap, format);
	__log(LOG_LEVEL_DEBUG, format, ap);
	va_end(ap);
}

void myLogError(const char *format, ...)
{
	va_list ap;

	va_start(ap, format);
	__log(LOG_LEVEL_ERROR, format, ap);
	va_end(ap);
}

void myLogRaw(const char *format, ...)
{
	va_list ap;

	va_start(ap, format);
	__log(LOG_LEVEL_RAW, format, ap);
	va_end(ap);
}

void myLogConsole(const char *format, ...)
{
	va_list ap;

	va_start(ap, format);
	__log(LOG_LEVEL_CONSOLE, format, ap);
	va_end(ap);
}