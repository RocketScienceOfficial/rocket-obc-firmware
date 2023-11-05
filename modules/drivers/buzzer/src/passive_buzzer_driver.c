#include "drivers/buzzer/passive_buzzer_driver.h"

#define DUTY_CYCLE 0.5f

FUNCRESULT passiveBuzzerInit(PWMConfig *config, PinNumber pin, UINT32 frequency)
{
    if (!config || FUNCFAILED(pwmInit(config, pin, frequency)))
    {
        return ERR_FAIL;
    }

    return SUC_OK;
}

FUNCRESULT passiveBuzzerSetActive(PWMConfig *config, BOOL active)
{
    return pwmSetDuty(config, active ? DUTY_CYCLE : 0.0f);
}