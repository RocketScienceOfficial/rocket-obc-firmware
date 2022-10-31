#pragma once

#include "shared/measurements_utils.h"
#include "tools/typedefs.h"

typedef struct ReceiverSendData
{
    MeasurementData measurement;
} ReceiverSendData;

VOID initializeReceiverLogger();
VOID logReceiverData(ReceiverSendData *data);