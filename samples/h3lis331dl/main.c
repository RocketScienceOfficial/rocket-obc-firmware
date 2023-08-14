#include "drivers/accelerometer/h3lis331dl_driver.h"
#include "drivers/tools/time_tracker.h"
#include "drivers/tools/board_control.h"
#include <stdio.h>

#define I2C 1
#define SDA 14
#define SCL 13

int main()
{
    boardInit(5000);

    i2cInitAll(I2C, 400000);

    H3lis331dlConfig h3lis331dlConfig = {0};
    h3lis331dlInitI2C(&h3lis331dlConfig, I2C, SDA, SCL);
    h3lis331dlSetPowerMode(&h3lis331dlConfig, H3LIS331DL_POWER_NORMAL);
    h3lis331dlSetRange(&h3lis331dlConfig, H3LIS331DL_RANGE_100G);
    h3lis331dlSetODR(&h3lis331dlConfig, H3LIS331DL_ODR_50HZ);

    while (TRUE)
    {
        vec3 accel = {0};
        h3lis331dlRead(&h3lis331dlConfig, &accel);

        printf("X: %f, Y: %f, Z: %f\n", accel.x, accel.y, accel.z);

        sleepMiliseconds(100);
    }

    return 0;
}