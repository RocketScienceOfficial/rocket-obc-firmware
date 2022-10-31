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

VOID logReceiverData(ReceiverSendData *data)
{
    MY_LOG_RECEIVER_DATA_BEGIN();
    MY_LOG_RECEIVER_DATA_FLOAT(data->measurement.accelerometerData.accel_x);
    MY_LOG_RECEIVER_DATA_FLOAT(data->measurement.accelerometerData.accel_y);
    MY_LOG_RECEIVER_DATA_FLOAT(data->measurement.accelerometerData.accel_z);
    MY_LOG_RECEIVER_DATA_FLOAT(data->measurement.accelerometerData.rot_x);
    MY_LOG_RECEIVER_DATA_FLOAT(data->measurement.accelerometerData.rot_y);
    MY_LOG_RECEIVER_DATA_FLOAT(data->measurement.accelerometerData.rot_z);
    MY_LOG_RECEIVER_DATA_INT(data->measurement.barometerData.pressure);
    MY_LOG_RECEIVER_DATA_FLOAT(data->measurement.barometerData.temperature);
    MY_LOG_RECEIVER_DATA_END();
}