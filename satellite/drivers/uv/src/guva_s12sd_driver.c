#include "drivers/uv/guva_s12sd_driver.h"
#include "drivers/gpio/gpio_typedefs.h"

FUNCRESULT guva_s12sd_Init(ADCInput input)
{
    if (FUNCFAILED(adcInitAll()) || FUNCFAILED(adcInitPin(input)))
    {
        return ERR_FAIL;
    }
    else
    {
        return SUC_OK;
    }
}

FUNCRESULT guva_s12sd_ReadUVIndex(ADCInput input, UINT32 *uvIndex)
{
    VoltageLevel voltage = 0;

    if (FUNCFAILED(adcRead(input, &voltage)))
    {
        return ERR_FAIL;
    }

    *uvIndex = voltage / 0.1;

    return SUC_OK;
}