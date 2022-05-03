#pragma once

typedef struct mg995_data
{
    int pin;
    float angle;
} mg995_data_t;

void mg995Init(mg995_data_t *data);
void mg995RotateAngle(mg995_data_t *data, float destAngleDegrees);