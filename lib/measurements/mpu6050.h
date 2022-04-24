#pragma once

#include <stdio.h>
#include "pinout_config.h"

#define PICO_DEFAULT_I2C MPU6050_I2C
#define PICO_DEFAULT_I2C_SDA_PIN MPU6050_I2C_SDA_PIN
#define PICO_DEFAULT_I2C_SCL_PIN MPU6050_I2C_SCL_PIN

#define ACCEL_FACTOR (0.000061)
#define GYRO_FACTOR (0.007633)
#define ADDR 0x68

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

void mpu6050Init();
void mpu6050ReadRaw(mpu6050_data_t* data);
void mpu6050ConvertData(mpu6050_data_t* data);