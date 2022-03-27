#ifndef MPU_6050_CONTROLLER_H
#define MPU_6050_CONTROLLER_H

#define ACCEL_FACTOR (0.000061)
#define GYRO_FACTOR (0.007633)

#include <stdio.h>

void mpu6050_init();
void mpu6050_get_values(double *accel_x, double *accel_y, double *accel_z, double *rot_x, double *rot_y, double *rot_z, double *temperature);

#endif