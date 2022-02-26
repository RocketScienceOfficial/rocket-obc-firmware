#ifndef MPU_6050_CONTROLLER_H
#define MPU_6050_CONTROLLER_H

#include <stdio.h>

void mpu6050_init();
void mpu6050_get_values(int16_t *accel_x, int16_t *accel_y, int16_t *accel_z, int16_t *rot_x, int16_t *rot_y, int16_t *rot_z, int16_t *temperature);

#endif