#include "drivers/battery/battery_driver.h"

FUNCRESULT batteryInit(BatteryConfig *config, ADCInput input, BatteryInterval *intervals, SIZE8 intervalsCount)
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
    config->_intervals = intervals;
    config->_intervalsCount = intervalsCount;

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

    for (SIZE8 i = 0; i < config->_intervalsCount; i++)
    {
        if (voltage >= config->_intervals[i].minVolts && voltage <= config->_intervals[i].maxVolts)
        {
            *percentage = (config->_intervals[i].maxPercent - config->_intervals[i].minPercent) / (config->_intervals[i].maxVolts - config->_intervals[i].minVolts) * (voltage - config->_intervals[i].minVolts) + config->_intervals[i].minPercent;

            return SUC_OK;
        }
    }

    return ERR_UNEXPECTED;
}