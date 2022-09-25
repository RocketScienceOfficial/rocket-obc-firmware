#include "kernel/logging/recorder.h"

TIME functionProfileBegin(STRING function)
{
	MY_LOG_RECORD("Begining execution of a function: '%s'", function);

	return getMsSinceBoot();
}

VOID functionProfileEnd(STRING function, TIME beginResult)
{
	TIME elapsed = getMsSinceBoot() - beginResult;

	MY_LOG_RECORD("Execution of function: '%s' took %u ms", function, elapsed);
}

Logger *myLogGetRecordLogger()
{
	static Logger logger;
	static BOOL loggerInitialized;

	if (!loggerInitialized)
	{
		myLogCreateLogger(&logger, MY_LOG_RECORD_NAME);

		loggerInitialized = TRUE;
	}

	return &logger;
}