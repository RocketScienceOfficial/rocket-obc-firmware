#include "drivers/cpu/cpu_temp_driver.h"
#include "drivers/gpio/gpio_driver.h"
#include "drivers/gpio/adc_driver.h"

FUNCRESULT cpuTempInit()
{
    if (FUNCFAILED(adcInitPin(ADC_TEMP_INPUT)))
    {
        return ERR_FAIL;
    }
    else
    {
        return SUC_OK;
    }
}

FUNCRESULT cpuTempReadTemp(FLOAT *temp)
{
    if (!temp)
    {
        return ERR_INVALIDARG;
    }

    VoltageLevel voltage = 0;

    if (FUNCFAILED(adcRead(ADC_TEMP_INPUT, &voltage)))
    {
        return ERR_FAIL;
    }

    *temp = 27 - (voltage - 0.706) / 0.001721;

    return SUC_OK;
}