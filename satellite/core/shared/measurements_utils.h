#pragma once

#include "barometer.h"
#include "accelerometer.h"
#include "gps.h"

typedef enum ComponentsStates
{
    COMPONENT_BAROMETER = 1 << 0,
    COMPONENT_ACCELEROMETER = 1 << 1,
    COMPONENT_GPS = 1 << 2,
} ComponentsStates;

typedef struct MeasurementData
{
    BarometerData barometerData;
    AccelerometerData accelerometerData;
    GPSData gpsData;
    int componentsStatus;
} MeasurementData;