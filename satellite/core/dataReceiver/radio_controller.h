#pragma once

#include "measurements_utils.h"
#include <stdbool.h>

void initializeRadio();
bool checkRadioPacket(MeasurementData *data_out, int *signalStrength);