#pragma once

#include <stdio.h>

#define MPU6050_ACCEL_FACTOR (0.000061)
#define MPU6050_GYRO_FACTOR (0.007633)

typedef struct mpu6050_data
{
    double accel_x;
    double accel_y;
    double accel_z;
    double rot_x;
    double rot_y;
    double rot_z;
    double temperature;
} mpu6050_data_t;

void mpu6050Init(int i2c, int sda, int scl);
void mpu6050ReadRaw(mpu6050_data_t* data);
void mpu6050ConvertData(mpu6050_data_t* data);