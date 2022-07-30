#pragma once

#include "barometer.h"
#include "accelerometer.h"
#include "gps.h"

enum components_states
{
    COMPONENT_BAROMETER = 1 << 0,
    COMPONENT_ACCELEROMETER = 1 << 1,
    COMPONENT_GPS = 1 << 2,
};

typedef struct measurement_data
{
    barometer_data_t barometerData;
    accelerometer_data_t accelerometerData;
    gps_data_t gpsData;
    int componentsStatus;
} measurement_data_t;