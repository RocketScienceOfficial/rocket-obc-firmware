#include "logging/logger.h"
#include "logging/log_serial.h"
#include "utils/time_tracker.h"
#include "myhardware/flash.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#define REPORT_ERROR(msg) logSerialError(msg)
#define LOG_HW_CALL(func)                                                                                    \
	{                                                                                                        \
		FUNCRESULT result = func;                                                                            \
		if (FUNCFAILED(result))                                                                              \
		{                                                                                                    \
			logSerialError("Hardware function with code failed: %d at '%s:%d'", result, __FILE__, __LINE__); \
		}                                                                                                    \
	}

void myLogCreateLogger(Logger *logger, const char *name)
{
	if (!logger || !name)
	{
		REPORT_ERROR("Invalid logger or name");

		return;
	}

	logger->_name = name;
	logger->_numSinks = 0;
}

void myLogCreateConsoleSink(Logger *logger, const char *pattern)
{
	if (!logger || !pattern)
	{
		REPORT_ERROR("Invalid logger or pattern");

		return;
	}

	LogSinkData sink = {
		._pattern = pattern,
		._type = SINK_CONSOLE,
		._customData = NULL,
	};

	logger->_sinks[logger->_numSinks++] = sink;
}

void myLogCreateFileSink(Logger *logger, const char *pattern, size_t fileIndex)
{
	if (!logger || !pattern || fileIndex >= FLASH_FILES_COUNT)
	{
		REPORT_ERROR("Invalid logger, pattern or fileName");

		return;
	}

	LogSinkData sink = {
		._pattern = pattern,
		._type = SINK_FILE,
		._customData = (size_t)fileIndex,
	};

	LOG_HW_CALL(flashClearFile(getDefaultFlashModule(), fileIndex));

	logger->_sinks[logger->_numSinks++] = sink;
}

char *parseLog(const char *loggerName, const char *pattern, const char *level, const char *format, va_list args)
{
	char *log = (char *)malloc(sizeof(char) * MAX_LOG_SIZE);
	size_t logIndex = 0;
	Timer timeMs = getMsSinceBoot();
	Timer minutes = floor(timeMs / 60000);
	Timer seconds = floor(timeMs / 1000 - minutes * 60);
	Timer miliseconds = floor(timeMs - minutes * 60000 - seconds * 1000);

	for (size_t i = 0; pattern[i] != '\0'; i++)
	{
		if (logIndex >= MAX_LOG_SIZE)
		{
			REPORT_ERROR("Log is too big");

			return NULL;
		}

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

static void __log(Logger *logger, const char *level, const char *format, va_list args)
{
	for (size_t i = 0; i < logger->_numSinks; i++)
	{
		char *log = parseLog(logger->_name, logger->_sinks[i]._pattern, level, format, args);

		if (log)
		{
			switch (logger->_sinks[i]._type)
			{
			case SINK_CONSOLE:
				logSerial(log);
				break;
			case SINK_FILE:
				LOG_HW_CALL(flashWriteFile(getDefaultFlashModule(), (size_t)logger->_sinks[i]._customData, log));
				break;
			default:
				REPORT_ERROR("Unknown sink type");
				break;
			}

			free(log);
		}
	}
}

void myLog(Logger *logger, const char *level, const char *format, ...)
{
	va_list ap;

	va_start(ap, format);
	__log(logger, level, format, ap);
	va_end(ap);
}

Logger *myLogGetCoreLogger()
{
	static Logger logger;
	static int initialized;

	if (!initialized)
	{
		myLogCreateLogger(&logger, MY_LOG_CORE_NAME);

		initialized = 1;
	}

	return &logger;
}