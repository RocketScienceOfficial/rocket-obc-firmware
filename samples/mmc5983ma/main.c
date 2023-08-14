#include "drivers/magnetometer/mmc5983ma_driver.h"
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

    MMC5983MAConfig mmc5983maConfig = {0};
    mmc5983maInitI2C(&mmc5983maConfig, I2C, SDA, SCL);
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