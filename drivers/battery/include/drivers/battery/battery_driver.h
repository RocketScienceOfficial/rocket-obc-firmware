#pragma once

#include <obc/api.h>
#include "drivers/gpio/gpio_driver.h"
#include "drivers/gpio/adc_driver.h"

/**
 * @brief Battery interval data structure
 */
typedef struct BatteryInterval
{
    FLOAT minVolts;   /** Minimum voltage */
    FLOAT maxVolts;   /** Maximum voltage */
    FLOAT minPercent; /** Minimum percent*/
    FLOAT maxPercent; /** Maximum percent*/
} BatteryInterval;

/**
 * @brief Battery config data.
 */
typedef struct BatteryConfig
{
    ADCInput input;             /** ADC input */
    BatteryInterval *intervals; /** Battery intervals */
    SIZE8 intervalsCount;       /** Battery intervals count */
} BatteryConfig;

/**
 * @brief Initialize battery
 *
 * @param config Battery data pointer
 * @param input ADC input
 * @param intervals Intervals
 * @param intervalsCount Count of intervals
 * @return Result code
 */
FUNCRESULT batteryInit(BatteryConfig *config, ADCInput input, BatteryInterval *intervals, SIZE8 intervalsCount);

/**
 * @brief Read battery percentage.
 *
 * @param config Battery data pointer.
 * @param percentage Percentage of battery.
 * @return Result code.
 */
FUNCRESULT batteryReadPercent(BatteryConfig *config, FLOAT *percentage);