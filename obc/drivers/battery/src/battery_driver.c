#include "drivers/battery/battery_driver.h"

FUNCRESULT batteryInit(BatteryConfig *config, ADCInput input, VoltageLevel minVolts, VoltageLevel maxVolts)
{
    if (!config)
    {
        return ERR_INVALIDARG;
    }

    if (FUNCFAILED(adcInitPin(input)))
    {
        return ERR_FAIL;
    }

    config->_input = input;
    config->_minVolts = minVolts;
    config->_maxVolts = maxVolts;

    return SUC_OK;
}

FUNCRESULT batteryReadPercent(BatteryConfig *config, FLOAT *percentage)
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

    *percentage = 100 / (config->_maxVolts - config->_minVolts) * (voltage - config->_minVolts);

    return SUC_OK;
}