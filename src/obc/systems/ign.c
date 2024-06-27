#include "ign.h"
#include "board_config.h"
#include "sm.h"
#include "sensors.h"
#include "radio.h"
#include "../middleware/events.h"
#include "../middleware/syslog.h"
#include "hal/gpio_driver.h"
#include "hal/time_tracker.h"

#define SYSTEM_NAME "ign"
#define IGN_UP_TIME_MS 1000
#define DEST_HEIGHT 30

typedef struct ign_pin_data
{
    hal_pin_number_t pin;
    bool fired;
    msec_t fireTime;
    bool finished;
} ign_pin_data_t;

static bool s_Armed;
static ign_pin_data_t s_IGN1;
static ign_pin_data_t s_IGN2;
static ign_pin_data_t s_IGN3;
static ign_pin_data_t s_IGN4;

static void _init_pin(ign_pin_data_t *data, hal_pin_number_t pin);
static uint8_t _add_flag(const ign_pin_data_t *data, ign_flags_t flag);
static void _ign_fire(ign_pin_data_t *data);
static void _ign_update(ign_pin_data_t *data);

void ign_init(void)
{
    _init_pin(&s_IGN1, PIN_IGN_1);
    _init_pin(&s_IGN2, PIN_IGN_2);
    _init_pin(&s_IGN3, PIN_IGN_3);
    _init_pin(&s_IGN4, PIN_IGN_4);

    SYS_LOG("READY");
}

void ign_update(void)
{
    if (radio_get_parsed_data()->arm_enabled)
    {
        s_Armed = true;

        SYS_LOG("Armed igniters!");
    }
    else if (radio_get_parsed_data()->arm_disabled)
    {
        s_Armed = false;

        SYS_LOG("Igniters were disarmed!");
    }

    if (events_poll(MSG_SM_APOGEE_REACHED))
    {
        _ign_fire(&s_IGN1);

        if (sm_get_apogee() - sm_get_base_alt() <= DEST_HEIGHT)
        {
            _ign_fire(&s_IGN2);
        }
    }
    else if (sm_get_state() == FLIGHT_STATE_FREE_FALL)
    {
        if (sensors_get_frame()->baroHeight - sm_get_base_alt() <= DEST_HEIGHT)
        {
            _ign_fire(&s_IGN2);
        }
    }

    _ign_update(&s_IGN1);
    _ign_update(&s_IGN2);
    _ign_update(&s_IGN3);
    _ign_update(&s_IGN4);
}

bool ign_is_armed(void)
{
    return s_Armed;
}

uint8_t ign_get_flags(void)
{
    uint8_t flags = 0;

    flags |= _add_flag(&s_IGN1, IGN_FLAG_IGN_1_STATE);
    flags |= _add_flag(&s_IGN2, IGN_FLAG_IGN_2_STATE);
    flags |= _add_flag(&s_IGN3, IGN_FLAG_IGN_3_STATE);
    flags |= _add_flag(&s_IGN4, IGN_FLAG_IGN_4_STATE);

    return flags;
}

static void _init_pin(ign_pin_data_t *data, hal_pin_number_t pin)
{
    data->pin = pin;

    hal_gpio_init_pin(pin, GPIO_OUTPUT);
}

static uint8_t _add_flag(const ign_pin_data_t *data, ign_flags_t flag)
{
    return data->fired && !data->finished ? flag : 0;
}

static void _ign_fire(ign_pin_data_t *data)
{
    if (s_Armed && !data->fired && !data->finished)
    {
        data->fired = true;
        data->fireTime = hal_time_get_ms_since_boot();

        hal_gpio_set_pin_state(data->pin, GPIO_HIGH);

        SYS_LOG("Firing IGN...");
    }
}

static void _ign_update(ign_pin_data_t *data)
{
    if (s_Armed && data->fired && !data->finished)
    {
        if (hal_time_get_ms_since_boot() - data->fireTime >= IGN_UP_TIME_MS)
        {
            data->finished = true;

            hal_gpio_set_pin_state(data->pin, GPIO_LOW);

            SYS_LOG("IGN has been fired!");
        }
    }
}