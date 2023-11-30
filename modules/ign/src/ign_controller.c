#include "modules/ign/ign_controller.h"
#include "modules/logger/logger.h"
#include <string.h>

#define IGN_UP_TIME_MS 500

static void _ign_try_fire(ign_data_t *ign, ign_pin_data_t *data, msec_t delay);
static void _ign_try_update(ign_pin_data_t *data);

void ign_init(ign_data_t *data, const ign_pins_t *pins, const ign_settings_t *settings)
{
    data->armed = false;

    data->settings = *settings;

    data->onlyMain = true;
    data->twoStages = false;

    memset(&data->mainData, 0, sizeof(data->mainData));
    memset(&data->drougeData, 0, sizeof(data->drougeData));
    memset(&data->sepData, 0, sizeof(data->sepData));
    memset(&data->secondData, 0, sizeof(data->secondData));

    data->mainData.pin = pins->main;
    data->drougeData.pin = pins->drouge;
    data->sepData.pin = pins->sep;
    data->secondData.pin = pins->second;
}

void ign_arm(ign_data_t *data)
{
    data->armed = true;

    OBC_INFO("Armed igniters!");
}

void ign_update(ign_data_t *data, const flight_sm_data_t *sm)
{
    if (sm->apogeeReached)
    {
        if (data->onlyMain)
        {
            _ign_try_fire(data, &data->mainData, 0);
        }
        else
        {
            _ign_try_fire(data, &data->drougeData, 0);
        }
    }

    if (sm->lastAlt > data->settings.mainAlt && sm->state == FLIGHT_STATE_FREE_FALL)
    {
        if (data->onlyMain)
        {
            _ign_try_fire(data, &data->mainData, 0);
        }
    }

    if (sm->state == FLIGHT_STATE_FREE_FLIGHT)
    {
        if (data->twoStages)
        {
            _ign_try_fire(data, &data->sepData, 0);
            _ign_try_fire(data, &data->secondData, data->settings.secondIgnDelay);
        }
    }

    _ign_try_update(&data->mainData);
    _ign_try_update(&data->drougeData);
    _ign_try_update(&data->sepData);
    _ign_try_update(&data->secondData);
}

static void _ign_try_fire(ign_data_t *ign, ign_pin_data_t *data, msec_t delay)
{
    if (ign->armed)
    {
        if (!data->fired)
        {
            data->fired = true;
            data->delay = delay;
            data->time = hal_time_get_ms_since_boot();
        }
    }
}

static void _ign_try_update(ign_pin_data_t *data)
{
    msec_t diff = hal_time_get_ms_since_boot() - data->time;

    if (data->fired && !data->setHigh && diff >= data->delay)
    {
        data->setHigh = true;

        hal_gpio_set_pin_state(data->pin, GPIO_HIGH);
    }

    if (data->fired && !data->finished && diff >= data->delay + IGN_UP_TIME_MS)
    {
        data->finished = true;

        hal_gpio_set_pin_state(data->pin, GPIO_LOW);
    }
}