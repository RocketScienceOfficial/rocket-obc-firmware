#include "receiver_logger.h"
#include "logging/logger.h"

static Logger s_Logger;

#define MY_LOG_RECEIVER_DATA_PATTERN "%c"
#define MY_LOG_RECEIVER_DATA_NAME "RECEIVER_DATA_LOG"
#define MY_LOG_RECEIVER_DATA_BEGIN() myLog(&s_Logger, "", "/*")
#define MY_LOG_RECEIVER_DATA_FLOAT(val) myLog(&s_Logger, "", "%f,", val)
#define MY_LOG_RECEIVER_DATA_INT(val) myLog(&s_Logger, "", "%d,", val)
#define MY_LOG_RECEIVER_DATA_EMPTY() myLog(&s_Logger, "", ",")
#define MY_LOG_RECEIVER_DATA_END() myLog(&s_Logger, "", "*/\n")

void initializeReceiverLogger()
{
    myLogCreateLogger(&s_Logger, MY_LOG_RECEIVER_DATA_NAME);
    myLogCreateConsoleSink(&s_Logger, MY_LOG_RECEIVER_DATA_PATTERN);
}

void logReceiverData(ReceiverSendData *data)
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