#include "recorder.h"
#include "time_tracker.h"
#include <stdio.h>

unsigned int functionProfileBegin(const char *function)
{
	MY_LOG_RECORD("Begining execution of a function: '%s'", function);

	return getMsSinceBoot();
}

void functionProfileEnd(const char *function, unsigned int beginResult)
{
	unsigned int elapsed = getMsSinceBoot() - beginResult;

	MY_LOG_RECORD("Execution of function: '%s' took %u ms", function, elapsed);
}

static logger_data_t s_Logger;
static int s_LoggerInitialized;

logger_data_t *myLogGetRecordLogger()
{
	if (!s_LoggerInitialized)
	{
		myLogCreateLogger(&s_Logger, MY_LOG_RECORD_NAME);

		s_LoggerInitialized = 1;
	}

	return &s_Logger;
}