#pragma once

struct MeasurementData
{
    float pos_x;
    float pos_y;
    float pos_z;
    float roll;
    float pitch;
    float yaw;
    float latitude;
    float longitude;
    float altitude;
    float velocity;
};

void SetMeasurementData(MeasurementData *data);

MeasurementData GetCurrentMeasurement();