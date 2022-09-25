#pragma once

#include "tools/typedefs.h"
#include "drivers/gpio/adc_driver.h"

/**
 * @brief Initialize GUVA-S12SD.
 * 
 * @param input ADC input.
 * @return Result code.
 */
FUNCRESULT guva_s12sd_Init(ADCInput input);

/**
 * @brief Read UV Index.
 * 
 * @param input ADC input.
 * @param uvIndex UV Index.
 * @return Result code.
 */
FUNCRESULT guva_s12sd_ReadUVIndex(ADCInput input, UINT32 *uvIndex);