#include "drivers/accelerometer/h3lis331dl_driver.h"
#include "drivers/tools/time_tracker.h"
#include "drivers/tools/board_control.h"
#include <stdio.h>

#define SPI 0
#define SCK 18
#define MISO 16
#define MOSI 19
#define CS 1

int main()
{
    boardInit(5000);

    spiInitAll(SPI, 1000000);

    H3lis331dlConfig h3lis331dlConfig = {0};
    h3lis331dlInitSPI(&h3lis331dlConfig, SPI, MISO, MOSI, CS, SCK);
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