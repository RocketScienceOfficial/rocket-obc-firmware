#include "drivers/accelerometer/h3lis331dl_driver.h"
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
    h3lis331dlInit(&h3lis331dlConfig, SPI, MISO, MOSI, CS, SCK);
    h3lis331dlSetPower(&h3lis331dlConfig, TRUE);
    h3lis331dlSetRange(&h3lis331dlConfig, H3LIS331DL_RANGE_100G);
    h3lis331dlSetODR(&h3lis331dlConfig, H3LIS331DL_ODR_50HZ);

    while (TRUE)
    {
        H3lis331dlData accelData = {0};
        h3lis331dlRead(&h3lis331dlConfig, &accelData);

        sleep_ms(50);
    }

    return 0;
}