#include "drivers/magnetometer/mmc5983ma_driver.h"
#include "tools/time_tracker.h"
#include "tools/board_control.h"
#include <stdio.h>

#define SPI 0
#define SCK 18
#define MOSI 19
#define MISO 16
#define CS 12

int main()
{
    boardInit(5000);

    spiInitAll(SPI, 1 * 1000 * 1000);

    MMC5983MAConfig mmc5983maConfig = {0};
    mmc5983maInit(&mmc5983maConfig, SPI, MISO, MOSI, CS, SCK);
    mmc5983maSetContinuousModeODR(&mmc5983maConfig, MMC5983MA_ODR_200HZ);

    while (TRUE)
    {
        vec3 mag = {0};
        mmc5983maRead(&mmc5983maConfig, &mag);

        printf("Raw:0,0,0,0,0,0,%d,%d,%d\n", (INT32)mag.x, (INT32)mag.y, (INT32)mag.z);

        sleepMiliseconds(10);
    }

    return 0;
}