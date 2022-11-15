#include "drivers/potentiometer/potentiometer_driver.h"

FUNCRESULT potentiometerInit(PotentiometerConfig *config, ADCInput input)
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

FUNCRESULT potentiometerReadPercent(PotentiometerConfig *config, FLOAT *percentage)
{
    if (!config || !percentage)
    {
        return ERR_INVALIDARG;
    }

    VoltageLevel voltage = 0;

    if (FUNCFAILED(adcRead(config->_input, &voltage)))
    {
        return ERR_FAIL;
    }

    *percentage = 100 / 3.3f * voltage;

    return SUC_OK;
}