#pragma once

#include <stdio.h>

typedef struct accelerometer_raw_data
{
    int16_t accel_x;
    int16_t accel_y;
    int16_t accel_z;
    int16_t rot_x;
    int16_t rot_y;
    int16_t rot_z;
    int16_t temperature;
} accelerometer_raw_data_t;

typedef struct accelerometer_data
{
    float accel_x;
    float accel_y;
    float accel_z;
    float rot_x;
    float rot_y;
    float rot_z;
    float temperature;
} accelerometer_data_t;

int accelerometerInit(int i2c, int sda, int scl, int lpFilter, int accelSensLevel, int gyroSensLevel);
int accelerometerCheck();
void accelerometerReadRaw(accelerometer_raw_data_t *data);
void accelerometerConvertData(accelerometer_raw_data_t *inData, accelerometer_data_t *outData);