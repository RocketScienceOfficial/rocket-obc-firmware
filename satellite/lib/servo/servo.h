#pragma once

typedef struct servo_data
{
    int pin;
    float angle;
} servo_data_t;

void servoInit(servo_data_t *data);
void servoRotateAngle(servo_data_t *data, float destAngleDegrees);