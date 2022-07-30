#include "receiver_logger.h"
#include <stdio.h>

static void logCallback(logger_data_t *logger, const char *level, const char *msg)
{
    printf(msg);
}

void initializeReceiverLogger()
{
    myLogCreateSink(myLogGetReceiverDataLogger(), &logCallback, MY_LOG_RECEIVER_DATA_PATTERN);
}

void logReceiverData(receiver_send_data_t *data)
{
    MY_LOG_RECEIVER_DATA_BEGIN();

    if (data->measurement.componentsStatus & COMPONENT_ACCELEROMETER)
    {
        MY_LOG_RECEIVER_DATA_FLOAT(data->measurement.accelerometerData.accel_x);
        MY_LOG_RECEIVER_DATA_FLOAT(data->measurement.accelerometerData.accel_y);
        MY_LOG_RECEIVER_DATA_FLOAT(data->measurement.accelerometerData.accel_z);
        MY_LOG_RECEIVER_DATA_FLOAT(data->measurement.accelerometerData.rot_x);
        MY_LOG_RECEIVER_DATA_FLOAT(data->measurement.accelerometerData.rot_y);
        MY_LOG_RECEIVER_DATA_FLOAT(data->measurement.accelerometerData.rot_z);
    }
    else
    {
        MY_LOG_RECEIVER_DATA_EMPTY();
        MY_LOG_RECEIVER_DATA_EMPTY();
        MY_LOG_RECEIVER_DATA_EMPTY();
        MY_LOG_RECEIVER_DATA_EMPTY();
        MY_LOG_RECEIVER_DATA_EMPTY();
        MY_LOG_RECEIVER_DATA_EMPTY();
    }

    if (data->measurement.componentsStatus & COMPONENT_BAROMETER)
    {
        MY_LOG_RECEIVER_DATA_INT(data->measurement.barometerData.pressure);
        MY_LOG_RECEIVER_DATA_FLOAT(data->measurement.barometerData.temperature);
    }
    else
    {
        MY_LOG_RECEIVER_DATA_EMPTY();
        MY_LOG_RECEIVER_DATA_EMPTY();
    }

    if (data->measurement.componentsStatus & COMPONENT_GPS)
    {
        MY_LOG_RECEIVER_DATA_INT(data->measurement.gpsData.latitude);
        MY_LOG_RECEIVER_DATA_FLOAT(data->measurement.gpsData.longitude);
        MY_LOG_RECEIVER_DATA_FLOAT(data->measurement.gpsData.altitude);
    }
    else
    {
        MY_LOG_RECEIVER_DATA_EMPTY();
        MY_LOG_RECEIVER_DATA_EMPTY();
        MY_LOG_RECEIVER_DATA_EMPTY();
    }

    MY_LOG_RECEIVER_DATA_FLOAT(data->condition.measureRAMUsagePercent);
    MY_LOG_RECEIVER_DATA_FLOAT(data->condition.receiverRAMUsagePercent);
    MY_LOG_RECEIVER_DATA_INT(data->condition.radioSignalStrength);

    MY_LOG_RECEIVER_DATA_END();
}

static logger_data_t s_Logger;
static int s_Initialized;

logger_data_t *myLogGetReceiverDataLogger()
{
    if (!s_Initialized)
    {
        myLogCreateLogger(&s_Logger, MY_LOG_RECEIVER_DATA_NAME);

        s_Initialized = 1;
    }

    return &s_Logger;
}