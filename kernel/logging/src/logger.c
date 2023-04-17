#include "kernel/logging/logger.h"
#include "drivers/console/console_output.h"
#include "tools/time_tracker.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#define REPORT_ERROR(msg) consoleLogError(msg)

VOID myLogCreateLogger(Logger *logger, const STRING name)
{
	if (!logger || !name)
	{
		REPORT_ERROR("Invalid logger or name");

		return;
	}

	logger->_name = name;
	logger->_numSinks = 0;
}

VOID myLogCreateConsoleSink(Logger *logger, const STRING pattern)
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

STRING parseLog(const STRING loggerName, const STRING pattern, const STRING level, const STRING format, va_list args)
{
	STRING log = (STRING)malloc(sizeof(CHAR) * MAX_LOG_SIZE);
	SIZE logIndex = 0;
	TIME timeMs = getMsSinceBoot();
	TIME minutes = floor(timeMs / 60000);
	TIME seconds = floor(timeMs / 1000 - minutes * 60);
	TIME miliseconds = floor(timeMs - minutes * 60000 - seconds * 1000);

	for (SIZE i = 0; pattern[i] != '\0'; i++)
	{
		if (logIndex >= MAX_LOG_SIZE)
		{
			REPORT_ERROR("Log is too big");

			return NULL;
		}

		if (pattern[i] == '%')
		{
			SIZE tmpI = i + 1;

			if (pattern[tmpI] == 'M')
			{
				CHAR buff[4];

				SIZE len = snprintf(buff, 4, "%u", minutes);

				for (SIZE j = 0; j < len; j++)
				{
					log[logIndex] = buff[j];
					logIndex++;
				}
			}
			else if (pattern[tmpI] == 'S')
			{
				CHAR buff[4];

				SIZE len = snprintf(buff, 4, "%u", seconds);

				for (SIZE j = 0; j < len; j++)
				{
					log[logIndex] = buff[j];
					logIndex++;
				}
			}
			else if (pattern[tmpI] == 'm')
			{
				CHAR buff[4];

				SIZE len = snprintf(buff, 4, "%u", miliseconds);

				for (SIZE j = 0; j < len; j++)
				{
					log[logIndex] = buff[j];
					logIndex++;
				}
			}
			else if (pattern[tmpI] == 'l')
			{
				for (SIZE j = 0; level[j] != '\0'; j++)
				{
					log[logIndex] = level[j];
					logIndex++;
				}
			}
			else if (pattern[tmpI] == 'c')
			{
				SIZE len = vsnprintf(NULL, 0, format, args) + 1;
				STRING msg = malloc(len);

				vsnprintf(msg, len, format, args);

				for (SIZE j = 0; msg[j] != '\0'; j++)
				{
					log[logIndex] = msg[j];
					logIndex++;
				}

				free(msg);
			}
			else if (pattern[tmpI] == 't')
			{
				for (SIZE j = 0; loggerName[j] != '\0'; j++)
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

static VOID __log(Logger *logger, const STRING level, const STRING format, va_list args)
{
	for (SIZE i = 0; i < logger->_numSinks; i++)
	{
		STRING log = parseLog(logger->_name, logger->_sinks[i]._pattern, level, format, args);

		if (log)
		{
			switch (logger->_sinks[i]._type)
			{
			case SINK_CONSOLE:
				consoleLog(log);
				break;
			default:
				REPORT_ERROR("Unknown sink type");
				break;
			}

			free(log);
		}
	}
}

VOID myLog(Logger *logger, const STRING level, const STRING format, ...)
{
	va_list ap;

	va_start(ap, format);
	__log(logger, level, format, ap);
	va_end(ap);
}

Logger *myLogGetCoreLogger()
{
	static Logger logger;
	static BOOL initialized;

	if (!initialized)
	{
		myLogCreateLogger(&logger, MY_LOG_CORE_NAME);

		initialized = TRUE;
	}

	return &logger;
}