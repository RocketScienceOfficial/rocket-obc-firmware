#include "drivers/battery/battery_driver.h"

FUNCRESULT batteryInit(ADCInput input, BatteryData *data, VoltageLevel minVolts, VoltageLevel maxVolts)
{
    if (!data)
    {
        return ERR_INVALIDARG;
    }

    if (FUNCFAILED(adcInitAll()) || FUNCFAILED(adcInitPin(input)))
    {
        return ERR_FAIL;
    }

    data->_minVolts = minVolts;
    data->_maxVolts = maxVolts;

    return SUC_OK;
}

FUNCRESULT batteryReadPercent(ADCInput input, BatteryData *data, FLOAT *percentage)
{
    VoltageLevel voltage = 0;

    if (FUNCFAILED(adcRead(input, &voltage)))
    {
        return ERR_FAIL;
    }

    *percentage = 100 / (data->_maxVolts - data->_minVolts) * (voltage - data->_minVolts);

    return SUC_OK;
}