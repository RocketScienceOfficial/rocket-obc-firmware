#include "services/logging/logger.h"
#include "drivers/console/console_output.h"
#include "drivers/tools/time_tracker.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#define REPORT_ERROR(msg) consoleLogError(msg)

static CHAR s_TempLog[MAX_LOG_SIZE];
static CHAR s_TempMsg[MAX_LOG_SIZE];

VOID myLogCreateLogger(Logger *logger, const STRING name)
{
	if (!logger || !name)
	{
		REPORT_ERROR("Invalid logger or name");

		return;
	}

	logger->name = name;
	logger->numSinks = 0;
}

VOID myLogCreateConsoleSink(Logger *logger, const STRING pattern)
{
	if (!logger || !pattern)
	{
		REPORT_ERROR("Invalid logger or pattern");

		return;
	}

	LogSinkData sink = {
		.pattern = pattern,
		.type = SINK_CONSOLE,
		.customData = NULL,
	};

	logger->sinks[logger->numSinks++] = sink;
}

VOID parseLog(const STRING loggerName, const STRING pattern, const STRING level, const STRING format, va_list args)
{
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

			return;
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
					s_TempLog[logIndex] = buff[j];
					logIndex++;
				}
			}
			else if (pattern[tmpI] == 'S')
			{
				CHAR buff[4];

				SIZE len = snprintf(buff, 4, "%u", seconds);

				for (SIZE j = 0; j < len; j++)
				{
					s_TempLog[logIndex] = buff[j];
					logIndex++;
				}
			}
			else if (pattern[tmpI] == 'm')
			{
				CHAR buff[4];

				SIZE len = snprintf(buff, 4, "%u", miliseconds);

				for (SIZE j = 0; j < len; j++)
				{
					s_TempLog[logIndex] = buff[j];
					logIndex++;
				}
			}
			else if (pattern[tmpI] == 'l')
			{
				for (SIZE j = 0; level[j] != '\0'; j++)
				{
					s_TempLog[logIndex] = level[j];
					logIndex++;
				}
			}
			else if (pattern[tmpI] == 'c')
			{
				SIZE len = vsnprintf(NULL, 0, format, args) + 1;

				vsnprintf(s_TempMsg, len, format, args);

				for (SIZE j = 0; s_TempMsg[j] != '\0'; j++)
				{
					s_TempLog[logIndex] = s_TempMsg[j];
					logIndex++;
				}
			}
			else if (pattern[tmpI] == 't')
			{
				for (SIZE j = 0; loggerName[j] != '\0'; j++)
				{
					s_TempLog[logIndex] = loggerName[j];
					logIndex++;
				}
			}

			i = tmpI;
		}
		else
		{
			s_TempLog[logIndex] = pattern[i];
			logIndex++;
		}
	}

	s_TempLog[logIndex] = '\0';
}

static VOID __log(Logger *logger, const STRING level, const STRING format, va_list args)
{
	for (SIZE i = 0; i < logger->numSinks; i++)
	{
		parseLog(logger->name, logger->sinks[i].pattern, level, format, args);

		if (s_TempLog)
		{
			switch (logger->sinks[i].type)
			{
			case SINK_CONSOLE:
				consoleLog(s_TempLog);
				break;
			default:
				REPORT_ERROR("Unknown sink type");
				break;
			}
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