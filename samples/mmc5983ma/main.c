#include "drivers/magnetometer/mmc5983ma_driver.h"
#include "pico/stdlib.h"
#include <stdio.h>

#define SPI 0
#define SCK 18
#define MOSI 19
#define MISO 16
#define CS 12

int main()
{
    stdio_init_all();
    sleep_ms(5000);

    spiInitAll(SPI, 1 * 1000 * 1000);

    MMC5983MAConfig mmc5983maConfig = {0};
    mmc5983maInit(&mmc5983maConfig, SPI, MISO, MOSI, CS, SCK);
    mmc5983maSetODR(&mmc5983maConfig, MMC5983MA_ODR_200HZ);

    while (TRUE)
    {
        MMC5983MAData magData = {0};
        mmc5983maRead(&mmc5983maConfig, &magData);
        
        printf("X: %f, Y: %f, Z: %f\n", magData.mag.x, magData.mag.y, magData.mag.z);

        sleep_ms(50);
    }

    return 0;
}