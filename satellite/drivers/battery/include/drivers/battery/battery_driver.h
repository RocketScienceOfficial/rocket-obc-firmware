#pragma once

#include "tools/typedefs.h"
#include "drivers/gpio/gpio_typedefs.h"
#include "drivers/gpio/adc_driver.h"

/**
 * @brief Data for battery use.
 */
typedef struct BatteryData
{
    /**
     * @brief Minimum voltage - 0%. INTERNAL USE!
     */
    VoltageLevel _minVolts;

    /**
     * @brief Maximum voltage - 100%. INTERNAL USE!
     */
    VoltageLevel _maxVolts;
} BatteryData;

/**
 * @brief Initialize battery.
 * 
 * @param input ADC input.
 * @param data Battery data pointer.
 * @param minVolts Minimum volts of battery - 0%.
 * @param maxVolts Maximum volts of battery - 100%.
 * @return Result code.
 */
FUNCRESULT batteryInit(ADCInput input, BatteryData *data, VoltageLevel minVolts, VoltageLevel maxVolts);

/**
 * @brief Read battery percentage.
 * 
 * @param input ADC input.
 * @param data Battery data pointer.
 * @param percentage percentage of battery.
 * @return Result code.
 */
FUNCRESULT batteryReadPercent(ADCInput input, BatteryData *data, FLOAT *percentage);