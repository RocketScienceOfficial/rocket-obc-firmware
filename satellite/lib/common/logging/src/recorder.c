#include "logging/recorder.h"
#include <stdio.h>

Timer functionProfileBegin(const char *function)
{
	MY_LOG_RECORD("Begining execution of a function: '%s'", function);

	return getMsSinceBoot();
}

void functionProfileEnd(const char *function, Timer beginResult)
{
	Timer elapsed = getMsSinceBoot() - beginResult;

	MY_LOG_RECORD("Execution of function: '%s' took %u ms", function, elapsed);
}

Logger *myLogGetRecordLogger()
{
	static Logger logger;
	static int loggerInitialized;

	if (!loggerInitialized)
	{
		myLogCreateLogger(&logger, MY_LOG_RECORD_NAME);

		loggerInitialized = 1;
	}

	return &logger;
}