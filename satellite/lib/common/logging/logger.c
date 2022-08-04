#include "logger.h"
#include "time_tracker.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

void myLogCreateLogger(logger_data_t *logger, const char *name)
{
	logger->numSinks = 0;
	logger->name = name;
}

void myLogCreateSink(logger_data_t *logger, logCallback_t callback, const char *pattern)
{
	logger->sinks[logger->numSinks] = (log_sink_data_t){.callback = callback, .pattern = pattern};
	logger->numSinks++;
}

char *parseLog(const char *loggerName, const char *pattern, const char *level, const char *format, va_list args)
{
	char *log = (char *)malloc(sizeof(char) * MAX_LOG_SIZE);
	size_t logIndex = 0;
	timer_t timeMs = getMsSinceBoot();
	timer_t minutes = floor(timeMs / 60000);
	timer_t seconds = floor(timeMs / 1000 - minutes * 60);
	timer_t miliseconds = floor(timeMs - minutes * 60000 - seconds * 1000);

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
				size_t len = vsnprintf(NULL, 0, format, args) + 1;
				char *msg = malloc(len);

				vsnprintf(msg, len, format, args);

				for (size_t j = 0; msg[j] != '\0'; j++)
				{
					log[logIndex] = msg[j];
					logIndex++;
				}

				free(msg);
			}
			else if (pattern[tmpI] == 't')
			{
				for (size_t j = 0; loggerName[j] != '\0'; j++)
				{
					log[logIndex] = loggerName[j];
					logIndex++;
				}
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
		char *log = parseLog(logger->name, logger->sinks[i].pattern, level, format, args);

		logger->sinks[i].callback(logger, level, log);

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

static logger_data_t s_CoreLogger;
static int s_CoreInitialized;

logger_data_t *myLogGetCoreLogger()
{
	if (!s_CoreInitialized)
	{
		myLogCreateLogger(&s_CoreLogger, MY_LOG_CORE_NAME);

		s_CoreInitialized = 1;
	}

	return &s_CoreLogger;
}