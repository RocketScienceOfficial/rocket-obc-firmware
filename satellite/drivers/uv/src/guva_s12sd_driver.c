#include "drivers/uv/guva_s12sd_driver.h"
#include "drivers/gpio/gpio_driver.h"

FUNCRESULT guva_s12sd_Init(GUVA_S12SD_Config *config, ADCInput input)
{
    if (!config)
    {
        return ERR_INVALIDARG;
    }

    if (FUNCFAILED(adcInitAll()) || FUNCFAILED(adcInitPin(input)))
    {
        return ERR_FAIL;
    }
    else
    {
        config->_input = input;

        return SUC_OK;
    }
}

FUNCRESULT guva_s12sd_ReadUVIndex(GUVA_S12SD_Config *config, UINT32 *uvIndex)
{
    if (!config || !uvIndex)
    {
        return ERR_INVALIDARG;
    }

    VoltageLevel voltage = 0;

    if (FUNCFAILED(adcRead(config->_input, &voltage)))
    {
        return ERR_FAIL;
    }

    *uvIndex = voltage / 0.1;

    return SUC_OK;
}