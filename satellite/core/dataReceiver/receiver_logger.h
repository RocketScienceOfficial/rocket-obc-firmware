#pragma once

#include "shared/measurements_utils.h"
#include "tools/typedefs.h"

typedef struct ReceiverInternalConditionData
{
    FLOAT measureRAMUsagePercent;
    FLOAT receiverRAMUsagePercent;
    FLOAT measureBatteryPercent;
    INT32 radioSignalStrength;
} ReceiverInternalConditionData;

typedef struct ReceiverSendData
{
    MeasurementData measurement;
    ReceiverInternalConditionData condition;
} ReceiverSendData;

VOID initializeReceiverLogger();
VOID logReceiverData(ReceiverSendData *data);