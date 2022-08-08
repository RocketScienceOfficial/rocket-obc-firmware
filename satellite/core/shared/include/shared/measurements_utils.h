#pragma once

#include "myhardware/barometer.h"
#include "myhardware/accelerometer.h"
#include "myhardware/gps.h"

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