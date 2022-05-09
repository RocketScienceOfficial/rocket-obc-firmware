#include "logger.h"
#include "logging_utils.h"
#include "time_tracker.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

void myLogCreateLogger(logger_data_t *logger)
{
	logger->numSinks = 0;
}

void myLogCreateSink(logger_data_t *logger, logCallback_t callback, const char *pattern)
{
	logger->sinks[logger->numSinks] = (log_sink_data_t){.callback = callback, .pattern = pattern};
	logger->numSinks++;
}

char *parseLog(const char *pattern, const char *level, const char *format, va_list args)
{
	char *log = (char *)malloc(sizeof(char) * 256);
	size_t logIndex = 0;
	unsigned int timeMs = getMsSinceBoot();
	unsigned int minutes = floor(timeMs / 60000);
	unsigned int seconds = floor(timeMs / 1000 - minutes * 60);
	unsigned int miliseconds = floor(timeMs - minutes * 60000 - seconds * 1000);

	for (size_t i = 0; pattern[i] != '\0'; i++)
	{
		if (pattern[i] == '%')
		{
			size_t tmpI = i + 1;

			if (pattern[tmpI] == 'M')
			{
				char buff[4];

				size_t len = snprintf(buff, 4, "%u", minutes);

				for (size_t j = 0; j < len; j++)
				{
					log[logIndex] = buff[j];
					logIndex++;
				}
			}
			else if (pattern[tmpI] == 'S')
			{
				char buff[4];

				size_t len = snprintf(buff, 4, "%u", seconds);

				for (size_t j = 0; j < len; j++)
				{
					log[logIndex] = buff[j];
					logIndex++;
				}
			}
			else if (pattern[tmpI] == 'm')
			{
				char buff[4];

				size_t len = snprintf(buff, 4, "%u", miliseconds);

				for (size_t j = 0; j < len; j++)
				{
					log[logIndex] = buff[j];
					logIndex++;
				}
			}
			else if (pattern[tmpI] == 'l')
			{
				for (size_t j = 0; level[j] != '\0'; j++)
				{
					log[logIndex] = level[j];
					logIndex++;
				}
			}
			else if (pattern[tmpI] == 'c')
			{
				char *msg = vlogBase(format, args);

				for (size_t j = 0; msg[j] != '\0'; j++)
				{
					log[logIndex] = msg[j];
					logIndex++;
				}

				free(msg);
			}

			i = tmpI;
		}
		else
		{
			log[logIndex] = pattern[i];
			logIndex++;
		}
	}

	log[logIndex] = '\0';

	return log;
}

static void __log(logger_data_t *logger, const char *level, const char *format, va_list args)
{
	for (size_t i = 0; i < logger->numSinks; i++)
	{
		char *log = parseLog(logger->sinks[i].pattern, level, format, args);

		logger->sinks[i].callback(level, log);

		free(log);
	}
}

void myLog(logger_data_t *logger, const char *level, const char *format, ...)
{
	va_list ap;

	va_start(ap, format);
	__log(logger, level, format, ap);
	va_end(ap);
}

void myLogInfo(logger_data_t *logger, const char *format, ...)
{
	va_list ap;

	va_start(ap, format);
	__log(logger, LOG_LEVEL_INFO, format, ap);
	va_end(ap);
}

void myLogDebug(logger_data_t *logger, const char *format, ...)
{
	va_list ap;

	va_start(ap, format);
	__log(logger, LOG_LEVEL_DEBUG, format, ap);
	va_end(ap);
}

void myLogError(logger_data_t *logger, const char *format, ...)
{
	va_list ap;

	va_start(ap, format);
	__log(logger, LOG_LEVEL_ERROR, format, ap);
	va_end(ap);
}

static logger_data_t s_CoreLogger;
static int s_CoreInitialized;

logger_data_t *myLogGetCoreLogger()
{
	if (!s_CoreInitialized)
	{
		myLogCreateLogger(&s_CoreLogger);

		s_CoreInitialized = 1;
	}

	return &s_CoreLogger;
}

static logger_data_t s_MeasureLogger;
static int s_MeasureInitialized;

logger_data_t *myLogGetMeasureLogger()
{
	if (!s_MeasureInitialized)
	{
		myLogCreateLogger(&s_MeasureLogger);

		s_MeasureInitialized = 1;
	}

	return &s_MeasureLogger;
}