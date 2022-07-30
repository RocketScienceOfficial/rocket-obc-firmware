#include "measurement_logger.h"

static logger_data_t s_MeasureLogger;
static int s_MeasureInitialized;

logger_data_t *myLogGetMeasureLogger()
{
	if (!s_MeasureInitialized)
	{
		myLogCreateLogger(&s_MeasureLogger, MY_LOG_MEASURE_NAME);

		s_MeasureInitialized = 1;
	}

	return &s_MeasureLogger;
}