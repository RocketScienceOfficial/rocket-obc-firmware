#include "logger.h"
#include "logging_utils.h"
#include "time_tracker.h"
#include <stdlib.h>
#include <stdio.h>

static event_handler_t s_eventHandler;

static void __log(const char *level, const char *format, va_list args)
{
	char *timestamp = getTimestamp();
	char *message = vlogBase(format, args);
	char *log = logBase(LOG_FORMAT, level, timestamp, message);
	void *data[1] = {log};

	callEvent(&s_eventHandler, data, 1);

	free(timestamp);
	free(message);
	free(log);
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