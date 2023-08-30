#include "measurements.h"
#include <Arduino.h>

static MeasurementData s_CurrentMeasurement;

void SetMeasurementData(MeasurementData *data)
{
    Serial.print("/*");
    Serial.print(data->pos_x);
    Serial.print(data->pos_y);
    Serial.print(data->pos_z);
    Serial.print(data->roll);
    Serial.print(data->pitch);
    Serial.print(data->yaw);
    Serial.print(data->latitude);
    Serial.print(data->longitude);
    Serial.print(data->altitude);
    Serial.print(data->velocity);
    Serial.println("*/");

    s_CurrentMeasurement = *data;
}

MeasurementData GetCurrentMeasurement()
{
    return s_CurrentMeasurement;
}