#include "recorder.h"
#include "time_tracker.h"

int functionProfileBegin(const char *function)
{
    return getMsSinceBoot();
}

void functionProfileEnd(const char *function, int beginResult)
{
    int elapsed = getMsSinceBoot() - beginResult;

    MY_LOG_RECORD("Execution of function: '%s' took %d ms", function, elapsed);
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