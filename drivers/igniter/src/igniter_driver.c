#include "drivers/igniter/igniter_driver.h"
#include "drivers/tools/time_tracker.h"

static TIME s_Offset;
static BOOL s_Fire;
static SIZE s_CurrentIgniter;

FUNCRESULT ignInit(IgniterData *data)
{
    for (SIZE i = 0; i < data->count; i++)
    {
        gpioInitPin(data->pins[i], GPIO_OUTPUT);
    }

    return SUC_OK;
}

FUNCRESULT ignFire(IgniterData *data)
{
    s_Offset = getMsSinceBoot();
    s_Fire = TRUE;
    s_CurrentIgniter = 0;

    return SUC_OK;
}

FUNCRESULT ignUpdate(IgniterData *data)
{
    if (!s_Fire || s_CurrentIgniter >= data->count)
    {
        return SUC_OK;
    }

    if (getMsSinceBoot() - s_Offset >= data->delay)
    {
        gpioSetPinState(data->pins[s_CurrentIgniter], GPIO_HIGH);

        s_Offset = getMsSinceBoot();
        s_CurrentIgniter++;
    }

    return SUC_OK;
}