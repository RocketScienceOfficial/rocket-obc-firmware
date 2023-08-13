#pragma once

#include <obc/api.h>
#include "measurements_utils.h"

VOID initAlgorithms(VOID);
VOID convertRawData(RawMeasurementData *raw, MeasurementData *data);