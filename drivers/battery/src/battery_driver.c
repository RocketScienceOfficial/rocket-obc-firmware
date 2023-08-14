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

    *percentage = batteryConvertVoltageToPercent(config->intervals, config->intervalsCount, voltage);

    return ERR_UNEXPECTED;
}

FLOAT batteryConvertVoltageToPercent(BatteryInterval *intervals, SIZE8 intervalsCount, VoltageLevel voltage)
{
    for (SIZE8 i = 0; i < intervalsCount; i++)
    {
        if (voltage >= intervals[i].minVolts && voltage <= intervals[i].maxVolts)
        {
            return (intervals[i].maxPercent - intervals[i].minPercent) / (intervals[i].maxVolts - intervals[i].minVolts) * (voltage - intervals[i].minVolts) + intervals[i].minPercent;
        }
    }

    return 0;
}