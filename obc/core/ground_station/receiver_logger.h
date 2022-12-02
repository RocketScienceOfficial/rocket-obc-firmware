#pragma once

#include "common/measurements_utils.h"
#include <obc/api.h>

typedef struct ReceiverSendData
{
    MeasurementData measurement;
} ReceiverSendData;

VOID initializeReceiverLogger();
VOID logReceiverData(ReceiverSendData *data);