#include "sd_manager.h"
#include "sd.h"
#include "measurement_logger.h"
#include "recorder.h"
#include "config.h"

void initializeSd()
{
    if (ENABLE_SD)
    {
        sdInit();

        sdInitFile(LOG_CORE_FILENAME);
        sdInitFile(LOG_MEASURE_FILENAME);
        sdInitFile(LOG_RECORD_FILENAME);

        sdFlush(LOG_CORE_FILENAME);
        sdFlush(LOG_MEASURE_FILENAME);
        sdFlush(LOG_RECORD_FILENAME);

        sdAttachToLogger(myLogGetCoreLogger(), MY_LOG_CORE_PATTERN, LOG_CORE_FILENAME);
        sdAttachToLogger(myLogGetMeasureLogger(), MY_LOG_MEASURE_PATTERN, LOG_MEASURE_FILENAME);
        sdAttachToLogger(myLogGetRecordLogger(), MY_LOG_RECORD_PATTERN, LOG_RECORD_FILENAME);
    }
}

void beginSd()
{
    if (ENABLE_SD)
    {
        sdBegin(LOG_CORE_FILENAME);
        sdBegin(LOG_MEASURE_FILENAME);
        sdBegin(LOG_RECORD_FILENAME);
    }
}

void endSd()
{
    if (ENABLE_SD)
    {
        sdEnd(LOG_CORE_FILENAME);
        sdEnd(LOG_MEASURE_FILENAME);
        sdEnd(LOG_RECORD_FILENAME);
    }
}