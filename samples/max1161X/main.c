#include "drivers/adc/max1161X_driver.h"
#include "drivers/tools/time_tracker.h"
#include "drivers/tools/board_control.h"
#include <stdio.h>

#define I2C 1
#define SDA 14
#define SCL 15

int main()
{
    boardInit(5000);

    i2cInitAll(I2C, 400000);

    MAX1161XConfig adcConfig = {0};
    max1161XInit(&adcConfig, MAX1161X_TYPE_3, I2C, SDA, SCL);

    while (TRUE)
    {
        VoltageLevel vs[3];

        max1161XRead(&adcConfig, MAX1161X_CHANNEL_AIN0, &vs[0]);
        max1161XRead(&adcConfig, MAX1161X_CHANNEL_AIN1, &vs[1]);
        max1161XRead(&adcConfig, MAX1161X_CHANNEL_AIN2, &vs[2]);

        printf("Voltages: %f, %f, %f\n", vs[0], vs[1], vs[2]);

        sleepMiliseconds(500);
    }

    return 0;
}