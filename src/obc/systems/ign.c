#include "ign.h"
#include "board_config.h"
#include "sensors.h"
#include "sm.h"
#include "hal/gpio_driver.h"
#include "hal/time_tracker.h"
#include <string.h>

#define ONLY_MAIN true
#define TWO_STAGES false
#define SECOND_DELAY 1000
#define IGN_UP_TIME_MS 1000

typedef struct ign_pin_data
{
    hal_pin_number_t pin;
    bool setHigh;
    bool fired;
    msec_t delay;
    msec_t time;
    bool finished;
} ign_pin_data_t;

static bool s_Armed;
static ign_pin_data_t s_MainData;
static ign_pin_data_t s_DrougeData;
static ign_pin_data_t s_SepData;
static ign_pin_data_t s_SecondData;

static void _ign_try_fire(ign_pin_data_t *data, msec_t delay);
static void _ign_try_update(ign_pin_data_t *data);

void ign_init(void)
{
    s_Armed = true;
    s_MainData.pin = PIN_IGN_1;
    s_DrougeData.pin = PIN_IGN_2;
    s_SepData.pin = PIN_IGN_3;
    s_SecondData.pin = PIN_IGN_4;
}

void ign_update(void)
{
    // if (sm_apogee_reached())
    // {
    //     if (ONLY_MAIN)
    //     {
    //         _ign_try_fire(&s_MainData, 0);
    //     }
    //     else
    //     {
    //         _ign_try_fire(&s_DrougeData, 0);
    //     }
    // }

    // if (sm->lastAlt > data->settings.mainAlt && sm->state == FLIGHT_STATE_FREE_FALL)
    // {
    //     if (ONLY_MAIN)
    //     {
    //         _ign_try_fire(&s_MainData, 0);
    //     }
    // }

    if (sm_get_state() == FLIGHT_STATE_FREE_FLIGHT)
    {
        if (TWO_STAGES)
        {
            _ign_try_fire(&s_SepData, 0);
            _ign_try_fire(&s_SecondData, SECOND_DELAY);
        }
    }

    _ign_try_update(&s_MainData);
    _ign_try_update(&s_DrougeData);
    _ign_try_update(&s_SepData);
    _ign_try_update(&s_SecondData);
}

static void _ign_try_fire(ign_pin_data_t *data, msec_t delay)
{
    if (s_Armed)
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
    if (s_Armed)
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
}