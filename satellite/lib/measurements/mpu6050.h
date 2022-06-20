#pragma once

#include <stdio.h>
#include "kalman_filter.h"

#define MPU6050_ACCEL_FACTOR (0.000061)
#define MPU6050_GYRO_FACTOR (0.007633)

typedef struct mpu6050_data
{
    float accel_x;
    float accel_y;
    float accel_z;
    float rot_x;
    float rot_y;
    float rot_z;
    float temperature;
} mpu6050_data_t;

int mpu6050Init(int i2c, int sda, int scl);
int mpu6050Check();
void mpu6050ReadRaw(mpu6050_data_t *data);
void mpu6050ConvertData(mpu6050_data_t *data);
void mpu6050FilterData(kalman_filter_data_t *kalmanData, mpu6050_data_t *data);