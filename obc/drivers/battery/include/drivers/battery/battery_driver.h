#pragma once

#include <obc/api.h>
#include "drivers/gpio/gpio_driver.h"
#include "drivers/gpio/adc_driver.h"

/**
 * @brief Battery config data.
 */
typedef struct BatteryConfig
{
    ADCInput _input;        /** ADC input. INTERNAL USE! */
    VoltageLevel _minVolts; /** Minimum voltage - 0%. INTERNAL USE! */
    VoltageLevel _maxVolts; /** Maximum voltage - 100%. INTERNAL USE! */
} BatteryConfig;

/**
 * @brief Initialize battery.
 *
 * @param config Battery data pointer.
 * @param input ADC input.
 * @param minVolts Minimum volts of battery - 0%.
 * @param maxVolts Maximum volts of battery - 100%.
 * @return Result code.
 */
FUNCRESULT batteryInit(BatteryConfig *config, ADCInput input, VoltageLevel minVolts, VoltageLevel maxVolts);

/**
 * @brief Read battery percentage.
 *
 * @param config Battery data pointer.
 * @param percentage Percentage of battery.
 * @return Result code.
 */
FUNCRESULT batteryReadPercent(BatteryConfig *config, FLOAT *percentage);