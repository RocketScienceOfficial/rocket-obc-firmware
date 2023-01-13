#include "receiver_logger.h"
#include "kernel/logging/logger.h"

static Logger s_Logger;

#define MY_LOG_RECEIVER_DATA_PATTERN "%c"
#define MY_LOG_RECEIVER_DATA_NAME "RECEIVER_DATA_LOG"
#define MY_LOG_RECEIVER_DATA_BEGIN() myLog(&s_Logger, "", "/*")
#define MY_LOG_RECEIVER_DATA_END() myLog(&s_Logger, "", "*/\n")
#define MY_LOG_RECEIVER_DATA_INTERNAL(val, format) myLog(&s_Logger, "", format, val)
#define MY_LOG_RECEIVER_DATA_FLOAT(val) MY_LOG_RECEIVER_DATA_INTERNAL(val, "%f,")
#define MY_LOG_RECEIVER_DATA_INT(val) MY_LOG_RECEIVER_DATA_INTERNAL(val, "%d,")
#define MY_LOG_RECEIVER_DATA_EMPTY() myLog(&s_Logger, "", ",")

VOID initializeReceiverLogger()
{
    myLogCreateLogger(&s_Logger, MY_LOG_RECEIVER_DATA_NAME);
    myLogCreateConsoleSink(&s_Logger, MY_LOG_RECEIVER_DATA_PATTERN);
}

VOID logReceiverData(MeasurementData *data)
{
    MY_LOG_RECEIVER_DATA_BEGIN();
    MY_LOG_RECEIVER_DATA_FLOAT(data->accel_x);
    MY_LOG_RECEIVER_DATA_FLOAT(data->accel_y);
    MY_LOG_RECEIVER_DATA_FLOAT(data->accel_z);
    MY_LOG_RECEIVER_DATA_FLOAT(data->vel_x);
    MY_LOG_RECEIVER_DATA_FLOAT(data->vel_y);
    MY_LOG_RECEIVER_DATA_FLOAT(data->vel_z);
    MY_LOG_RECEIVER_DATA_FLOAT(data->pos_x);
    MY_LOG_RECEIVER_DATA_FLOAT(data->pos_y);
    MY_LOG_RECEIVER_DATA_FLOAT(data->pos_z);
    MY_LOG_RECEIVER_DATA_FLOAT(data->roll);
    MY_LOG_RECEIVER_DATA_FLOAT(data->pitch);
    MY_LOG_RECEIVER_DATA_FLOAT(data->yaw);
    MY_LOG_RECEIVER_DATA_FLOAT(data->latitude);
    MY_LOG_RECEIVER_DATA_FLOAT(data->longtitude);
    MY_LOG_RECEIVER_DATA_FLOAT(data->altitude);
    MY_LOG_RECEIVER_DATA_FLOAT(data->temperature);
    MY_LOG_RECEIVER_DATA_FLOAT(data->pressure);
    MY_LOG_RECEIVER_DATA_FLOAT(data->humidity);
    MY_LOG_RECEIVER_DATA_INT(data->radioSignalStrength);
    MY_LOG_RECEIVER_DATA_FLOAT(data->cpuTemp);
    MY_LOG_RECEIVER_DATA_FLOAT(data->batteryPercentage);
    MY_LOG_RECEIVER_DATA_FLOAT(data->fuelPercentage);
    MY_LOG_RECEIVER_DATA_END();
}