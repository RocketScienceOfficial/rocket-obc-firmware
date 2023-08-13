#include "algorithms.h"
#include "config.h"
#include "ahrs/madgwick_filter.h"

static MadgiwckFilterData s_MadgwickFilterData;

VOID initAlgorithms(VOID)
{
    madgwickInit(&s_MadgwickFilterData, 5, 1.0f / MEASUREMENTS_UPDATE_RATE_MS);
}

VOID convertRawData(RawMeasurementData *raw, MeasurementData *data)
{
    vec3 gyro = {
        .x = raw->gyrolX_1,
        .y = raw->gyrolY_1,
        .z = raw->gyrolZ_1,
    };
    vec3 accel = {
        .x = raw->accelX_1,
        .y = raw->accelY_1,
        .z = raw->accelZ_1,
    };
    vec3 mag = {
        .x = raw->magX_1,
        .y = raw->magY_1,
        .z = raw->magZ_1,
    };
    vec3 eulerData = {0};

    madgwickUpdateMARG(&s_MadgwickFilterData, gyro, accel, mag);
    quatToEuler(&eulerData, &s_MadgwickFilterData.q);

    *data = (MeasurementData){
        .pos_x = 0,
        .pos_y = 0,
        .pos_z = 0,
        .latitude = raw->lat,
        .longitude = raw->lon,
        .altitude = raw->alt,
        .roll = eulerData.x,
        .pitch = eulerData.y,
        .yaw = eulerData.z,
        .velocity = 0,
    };
}