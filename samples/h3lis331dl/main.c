#include "drivers/accelerometer/h3lis331dl_driver.h"
#include "kernel/services/driver_calling.h"
#include "pico/stdlib.h"
#include <stdio.h>

#define SPI 0
#define SCK 18
#define MOSI 19
#define MISO 16
#define CS 1

int main()
{
    stdio_init_all();
    sleep_ms(5000);

    spiInitAll(0, 1 * 1000 * 1000);

    H3lis331dlConfig h3lis331dlConfig = {0};
    DRIVER_CALL(h3lis331dlInit(&h3lis331dlConfig, SPI, MISO, MOSI, CS, SCK));
    DRIVER_CALL(h3lis331dlSetPowerMode(&h3lis331dlConfig, H3LIS331DL_POWER_NORMAL));
    DRIVER_CALL(h3lis331dlSetRange(&h3lis331dlConfig, H3LIS331DL_RANGE_100G));
    DRIVER_CALL(h3lis331dlSetODR(&h3lis331dlConfig, H3LIS331DL_ODR_50HZ));

    while (TRUE)
    {
        vec3 accel = {0};
        DRIVER_CALL(h3lis331dlRead(&h3lis331dlConfig, &accel));

        printf("X: %f, Y: %f, Z: %f\n", accel.x, accel.y, accel.z);

        sleep_ms(100);
    }

    return 0;
}