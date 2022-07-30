#pragma once

#include "measurements_utils.h"

void initializeRadio();
int checkRadioPacket(measurement_data_t *data_out, int *signalStrength);