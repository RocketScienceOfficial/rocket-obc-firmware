#include "modules/drivers/igniter/igniter_driver.h"
#include "modules/drivers/hal/time_tracker.h"

static time_t s_Offset;
static bool s_Fire;
static size_t s_CurrentIgniter;

void ign_init(igniter_data_t *data)
{
    for (size_t i = 0; i < data->count; i++)
    {
        gpio_init_pin(data->pins[i], GPIO_OUTPUT);
    }
}

void ign_fire(igniter_data_t *data)
{
    s_Offset = time_get_ms_since_boot();
    s_Fire = true;
    s_CurrentIgniter = 0;
}

void ign_update(igniter_data_t *data)
{
    if (!s_Fire || s_CurrentIgniter >= data->count)
    {
        return;
    }

    if (time_get_ms_since_boot() - s_Offset >= data->delay)
    {
        gpio_set_pin_state(data->pins[s_CurrentIgniter], GPIO_HIGH);

        s_Offset = time_get_ms_since_boot();
        s_CurrentIgniter++;
    }
}