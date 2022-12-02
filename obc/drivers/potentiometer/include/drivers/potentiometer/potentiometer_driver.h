#pragma once

#include <obc/api.h>
#include "drivers/gpio/adc_driver.h"

/**
 * @brief Potentiometer config data.
 */
typedef struct PotentiometerConfig
{
    ADCInput _input; /** ADC input. INTERNAL USE! */
} PotentiometerConfig;

/**
 * @brief Initialize potentiometer.
 *
 * @param config Potentiometer data pointer.
 * @param input ADC input.
 * @return Result code.
 */
FUNCRESULT potentiometerInit(PotentiometerConfig *config, ADCInput input);

/**
 * @brief Read potentiometer percentage.
 *
 * @param config Potentiometer data pointer.
 * @param percentage Percentage of potentiometer.
 * @return Result code.
 */
FUNCRESULT potentiometerReadPercent(PotentiometerConfig *config, FLOAT *percentage);