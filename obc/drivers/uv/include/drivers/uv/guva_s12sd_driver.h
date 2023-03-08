#pragma once

#include <obc/api.h>
#include "drivers/gpio/adc_driver.h"

/**
 * @brief GUVA-S12SD config data
 */
typedef struct GUVA_S12SD_Config
{
    ADCInput _input; /** ADC input */
} GUVA_S12SD_Config;

/**
 * @brief Initialize GUVA-S12SD
 *
 * @param config GUVA-S12SD data pointer
 * @param input ADC input
 * @return Result code
 */
FUNCRESULT guva_s12sd_Init(GUVA_S12SD_Config *config, ADCInput input);

/**
 * @brief Read UV Index
 *
 * @param config GUVA-S12SD data pointer
 * @param uvIndex UV Index
 * @return Result code
 */
FUNCRESULT guva_s12sd_ReadUVIndex(GUVA_S12SD_Config *config, UINT32 *uvIndex);