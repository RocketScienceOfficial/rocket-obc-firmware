#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include "math.h"

#include "pico/time.h"

#include "logger.h"
#include "sd_card_controller.h"

static char *get_timestamp()
{
	uint32_t time_ms = to_ms_since_boot(get_absolute_time());
	uint minutes = floor(time_ms / 60000);
	uint seconds = floor(time_ms / 1000 - minutes * 60);
	uint miliseconds = floor(time_ms - minutes * 60000 - seconds * 1000);

	char *buffer = malloc(32);

	snprintf(buffer, 16, "%u:%u:%u", minutes, seconds, miliseconds);

	return buffer;
}

static void __log(int sdwrite, const char *level, const char *format, va_list args)
{
	char *timestamp = get_timestamp();
	int len = snprintf(NULL, 0, LOG_FORMAT, timestamp, level, format) + 1;
	char *newformat = malloc(len);

	snprintf(newformat, len, LOG_FORMAT, timestamp, level, format);

	int flen = vprintf(newformat, args) + 1;

	if (sdwrite > 0)
	{
		char buffer[128];
		vsnprintf(buffer, flen, newformat, args);

		sd_write(buffer, LOG_FILENAME);
	}

	free(timestamp);
	free(newformat);
}

static void __log_measure(const char *format, va_list args)
{
	int len = snprintf(NULL, 0, MEASURE_FORMAT, format) + 1;
	char *newformat = malloc(len);

	snprintf(newformat, len, MEASURE_FORMAT, format);

	int flen = vprintf(newformat, args) + 1;

	char buffer[128];
	vsnprintf(buffer, flen, newformat, args);

	sd_write(buffer, MEASURE_FILENAME);

	free(newformat);
}

void my_log(const char *format, ...)
{
	va_list ap;

	va_start(ap, format);
	__log(1, "DEBUG", format, ap);
	va_end(ap);
}

void my_log_error(const char *format, ...)
{
	va_list ap;

	va_start(ap, format);
	__log(1, "ERROR", format, ap);
	va_end(ap);
}

void my_log_notout(const char *format, ...)
{
	va_list ap;

	va_start(ap, format);
	__log(0, "DEBUG", format, ap);
	va_end(ap);
}

void my_log_error_notout(const char *format, ...)
{
	va_list ap;

	va_start(ap, format);
	__log(0, "ERROR", format, ap);
	va_end(ap);
}

void my_log_measure_init()
{
	my_log_measure("Time");
	my_log_measure("Pressure (hPa)");
	my_log_measure("Temperature (C)");
	my_log_measure_end();
}

void my_log_measure_start()
{
	char *timestamp = get_timestamp();
	int len = snprintf(NULL, 0, MEASURE_FORMAT, timestamp) + 1;
	char *newformat = malloc(len);

	snprintf(newformat, len, MEASURE_FORMAT, timestamp);

	int flen = printf(newformat, timestamp) + 1;

	char buffer[128];
	snprintf(buffer, flen, newformat, timestamp);

	sd_write(buffer, MEASURE_FILENAME);

	free(newformat);
	free(timestamp);
}

void my_log_measure(const char *format, ...)
{
	va_list ap;

	va_start(ap, format);
	__log_measure(format, ap);
	va_end(ap);
}

void my_log_measure_end()
{
	sd_write("\n", MEASURE_FILENAME);
}