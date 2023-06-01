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

    config->input = input;
    config->intervals = intervals;
    config->intervalsCount = intervalsCount;

    return SUC_OK;
}

FUNCRESULT batteryReadPercent(BatteryConfig *config, FLOAT *percentage)
{
    if (!config || !percentage)
    {
        return ERR_INVALIDARG;
    }

    VoltageLevel voltage = 0;

    if (FUNCFAILED(adcRead(config->input, &voltage)))
    {
        return ERR_FAIL;
    }

    for (SIZE8 i = 0; i < config->intervalsCount; i++)
    {
        if (voltage >= config->intervals[i].minVolts && voltage <= config->intervals[i].maxVolts)
        {
            *percentage = (config->intervals[i].maxPercent - config->intervals[i].minPercent) / (config->intervals[i].maxVolts - config->intervals[i].minVolts) * (voltage - config->intervals[i].minVolts) + config->intervals[i].minPercent;

            return SUC_OK;
        }
    }

    return ERR_UNEXPECTED;
}