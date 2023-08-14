#include "drivers/accelerometer/lsm6dso32_driver.h"
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

    LSM6DSO32Config config = {0};
    lsm6dso32InitI2C(&config, I2C, SDA, SCL);
    lsm6dso32SetODR(&config, LSM6DSO32_ODR_416HZ, LSM6DSO32_ODR_416HZ);
    lsm6dso32SetRange(&config, LSM6DSO32_RANGE_4G, LSM6DSO32_RANGE_125DPS);

    while (TRUE)
    {
        vec3 accel = {0};
        vec3 gyro = {0};
        FLOAT temp = 0;
        lsm6dso32Read(&config, &accel, &gyro, &temp);

        printf("ACC  X: %f, Y: %f, Z: %f\n", accel.x, accel.y, accel.z);
        printf("GYR  X: %f, Y: %f, Z: %f\n", gyro.x, gyro.y, gyro.z);
        printf("TMP  %f\n", temp);

        sleepMiliseconds(100);
    }

    return 0;
}