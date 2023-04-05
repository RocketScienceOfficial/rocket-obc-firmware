#include "drivers/battery/battery_driver.h"
#include "pico/stdlib.h"
#include <stdio.h>

#define BATTERY_ADC_CHANNEL 3
#define BATTERY_INTERVALS_COUNT 10

int main()
{
    stdio_init_all();
    sleep_ms(5000);

    adcInitAll();

    BatteryInterval batteryIntervals[BATTERY_INTERVALS_COUNT] = {
        {0.7273f, 0.7636f, 90.0f, 100.0f},
        {0.7182, 0.7273f, 80.0f, 90.0f},
        {0.6999f, 0.7182f, 70.0f, 80.0f},
        {0.6909f, 0.6999f, 60.0f, 70.0f},
        {0.6818f, 0.6909f, 50.0f, 60.0f},
        {0.6764, 0.6818f, 40.0f, 50.0f},
        {0.6727f, 0.6764f, 30.0f, 40.0f},
        {0.6673, 0.6727f, 20.0f, 30.0f},
        {0.6636f, 0.6673f, 10.0f, 20.0f},
        {0.5455f, 0.6636f, 0.0f, 10.0f},
    };
    BatteryConfig batteryConfig = {0};

    batteryInit(&batteryConfig, BATTERY_ADC_CHANNEL, batteryIntervals, BATTERY_INTERVALS_COUNT);

    while (TRUE)
    {
        FLOAT percent = -1;
        batteryReadPercent(&batteryConfig, &percent);

        printf("Battery: %f%\n", percent);

        sleep_ms(500);
    }

    return 0;
}