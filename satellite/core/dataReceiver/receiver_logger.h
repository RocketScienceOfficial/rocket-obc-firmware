#pragma once

#include "measurements_utils.h"
#include "logger.h"

typedef struct receiver_internal_condition_data
{
    float measureRAMUsagePercent;
    float receiverRAMUsagePercent;
    int radioSignalStrength;
} receiver_internal_condition_data_t;

typedef struct receiver_send_data
{
    measurement_data_t measurement;
    receiver_internal_condition_data_t condition;
} receiver_send_data_t;

void initializeReceiverLogger();
void logReceiverData(receiver_send_data_t *data);
logger_data_t *myLogGetReceiverDataLogger();

#define MY_LOG_RECEIVER_DATA_PATTERN "%c"
#define MY_LOG_RECEIVER_DATA_NAME "RECEIVER_DATA_LOG"
#define MY_LOG_RECEIVER_DATA_BEGIN() myLog(myLogGetReceiverDataLogger(), "", "/*")
#define MY_LOG_RECEIVER_DATA_FLOAT(val) myLog(myLogGetReceiverDataLogger(), "", "%f,", val)
#define MY_LOG_RECEIVER_DATA_INT(val) myLog(myLogGetReceiverDataLogger(), "", "%d,", val)
#define MY_LOG_RECEIVER_DATA_EMPTY() myLog(myLogGetReceiverDataLogger(), "", ",")
#define MY_LOG_RECEIVER_DATA_END() myLog(myLogGetReceiverDataLogger(), "", "*/\n")