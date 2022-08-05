#pragma once

#include "measurements_utils.h"

typedef struct ReceiverInternalConditionData
{
    float measureRAMUsagePercent;
    float receiverRAMUsagePercent;
    int radioSignalStrength;
} ReceiverInternalConditionData;

typedef struct ReceiverSendData
{
    MeasurementData measurement;
    ReceiverInternalConditionData condition;
} ReceiverSendData;

void initializeReceiverLogger();
void logReceiverData(ReceiverSendData *data);