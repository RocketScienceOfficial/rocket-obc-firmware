#include "ign.h"
#include "board_config.h"
#include "sm.h"
#include "sensors.h"
#include "serial.h"
#include "dataman.h"
#include "radio.h"
#include "../middleware/events.h"
#include "../middleware/syslog.h"
#include "hal/gpio_driver.h"
#include "hal/time_tracker.h"
#include <string.h>

#define SYSTEM_NAME "ign"
#define IGN_UP_TIME_MS 1000
#define IGN_FUSE_WORKING_IGN_PRESENT_FACTOR 0
#define IGN_FUSE_WORKING_IGN_NOT_PRESENT_FACTOR 0.0189607f
#define IGN_FUSE_NOT_WORKING_IGN_PRESENT_FACTOR 0.0297897f
#define IGN_FUSE_NOT_WORKING_IGN_NOT_PRESENT_FACTOR 0.0383104f
#define IGN_PIN_CHECK_EPS 0.005f

typedef struct ign_pin_data
{
    hal_pin_number_t pin;
    uint8_t contFlags;
    bool fired;
    msec_t fireTime;
    bool finished;
} ign_pin_data_t;

static bool s_Armed;
static ign_pin_data_t s_IGN1;
static ign_pin_data_t s_IGN2;
static ign_pin_data_t s_IGN3;
static ign_pin_data_t s_IGN4;

static void _run_control(void);
static void _run_logic(void);
static void _update_flags(void);
static void _init_pin(ign_pin_data_t *data, hal_pin_number_t pin);
static uint8_t _get_ign_flag(const ign_pin_data_t *data, ign_flags_t contFlag, ign_flags_t stateFlag);
static void _set_cont_flags(ign_pin_data_t *data, float v);
static void _ign_fire(ign_pin_data_t *data);
static void _ign_update(ign_pin_data_t *data);

void ign_init(void)
{
    _init_pin(&s_IGN1, PIN_IGN_EN_1);
    _init_pin(&s_IGN2, PIN_IGN_EN_2);
    _init_pin(&s_IGN3, PIN_IGN_EN_3);
    _init_pin(&s_IGN4, PIN_IGN_EN_4);

    SYS_LOG("READY");
}

void ign_update(void)
{
    _run_control();
    _run_logic();
    _update_flags();
}

bool ign_is_armed(void)
{
    return s_Armed;
}

uint8_t ign_get_flags(void)
{
    uint8_t flags = 0;

    flags |= _get_ign_flag(&s_IGN1, IGN_FLAG_IGN_1_CONT, IGN_FLAG_IGN_1_STATE);
    flags |= _get_ign_flag(&s_IGN2, IGN_FLAG_IGN_2_CONT, IGN_FLAG_IGN_2_STATE);
    flags |= _get_ign_flag(&s_IGN3, IGN_FLAG_IGN_3_CONT, IGN_FLAG_IGN_3_STATE);
    flags |= _get_ign_flag(&s_IGN4, IGN_FLAG_IGN_4_CONT, IGN_FLAG_IGN_4_STATE);

    return flags;
}

uint8_t ign_get_cont_flags(uint8_t ignNumber)
{
    return ignNumber == 1 ? s_IGN1.contFlags : ignNumber == 2 ? s_IGN2.contFlags
                                           : ignNumber == 3   ? s_IGN3.contFlags
                                           : ignNumber == 4   ? s_IGN4.contFlags
                                                              : 0;
}

static void _run_control(void)
{
    if (sm_get_state() == FLIGHT_STATE_STANDING)
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

        if (events_poll(MSG_CMD_IGN_TEST))
        {
            const char *data = serial_get_param_at_index(0);

            if (data)
            {
                int n = (int)(data[0] - '0');

                SYS_LOG("Testing igniter %d...", n);

                hal_pin_number_t pin = PIN_IGN_EN_1 - n;

                hal_gpio_set_pin_state(pin, GPIO_HIGH);
                hal_time_sleep_ms(1000);
                hal_gpio_set_pin_state(pin, GPIO_LOW);
            }

            hal_serial_printf("\\ign-test-finish\n");
        }
    }
}

static void _run_logic(void)
{
    if (s_Armed)
    {
        if (events_poll(MSG_SM_APOGEE_REACHED))
        {
            _ign_fire(&s_IGN1);

            if (sm_get_apogee() - sm_get_base_alt() <= dataman_get_config()->mainHeight)
            {
                _ign_fire(&s_IGN2);
            }
        }
        else if (sm_get_state() == FLIGHT_STATE_FREE_FALL)
        {
            if (sensors_get_frame()->baroHeight - sm_get_base_alt() <= dataman_get_config()->mainHeight)
            {
                _ign_fire(&s_IGN2);
            }
        }

        _ign_update(&s_IGN1);
        _ign_update(&s_IGN2);
        _ign_update(&s_IGN3);
        _ign_update(&s_IGN4);
    }
}

static void _update_flags(void)
{
    if (events_poll(MSG_SENSORS_ADC_READ))
    {
        _set_cont_flags(&s_IGN1, sensors_get_frame()->ignDet1Volts);
        _set_cont_flags(&s_IGN2, sensors_get_frame()->ignDet2Volts);
        _set_cont_flags(&s_IGN3, sensors_get_frame()->ignDet3Volts);
        _set_cont_flags(&s_IGN4, sensors_get_frame()->ignDet4Volts);
    }
}

static void _init_pin(ign_pin_data_t *data, hal_pin_number_t pin)
{
    memset(data, 0, sizeof(*data));

    data->pin = pin;

    hal_gpio_init_pin(pin, GPIO_OUTPUT);
    hal_gpio_set_pin_state(pin, GPIO_LOW);
}

static uint8_t _get_ign_flag(const ign_pin_data_t *data, ign_flags_t contFlag, ign_flags_t stateFlag)
{
    uint8_t cont = data->contFlags & IGN_CONT_FLAG_ENABLED ? contFlag : 0;
    uint8_t state = data->fired && !data->finished ? stateFlag : 0;

    return cont | state;
}

static void _set_cont_flags(ign_pin_data_t *data, float v)
{
    if (sensors_get_frame()->batPercent == 0 || (data->fired && !data->finished))
    {
        data->contFlags = 0;

        return;
    }

    float vref = sensors_get_frame()->batVolts;

    if (v < vref * (IGN_FUSE_WORKING_IGN_PRESENT_FACTOR + IGN_PIN_CHECK_EPS))
    {
        data->contFlags = IGN_CONT_FLAG_ENABLED | IGN_CONT_FLAG_IGN_PRESENT | IGN_CONT_FLAG_FUSE_WORKING;
    }
    else if (v < vref * (IGN_FUSE_WORKING_IGN_NOT_PRESENT_FACTOR + IGN_PIN_CHECK_EPS))
    {
        data->contFlags = IGN_CONT_FLAG_ENABLED | IGN_CONT_FLAG_FUSE_WORKING;
    }
    else if (v < vref * (IGN_FUSE_NOT_WORKING_IGN_PRESENT_FACTOR + IGN_PIN_CHECK_EPS))
    {
        data->contFlags = IGN_CONT_FLAG_ENABLED | IGN_CONT_FLAG_IGN_PRESENT;
    }
    else if (v < (vref * IGN_FUSE_NOT_WORKING_IGN_NOT_PRESENT_FACTOR + IGN_PIN_CHECK_EPS))
    {
        data->contFlags = IGN_CONT_FLAG_ENABLED;
    }
    else
    {
        data->contFlags = 0;
    }
}

static void _ign_fire(ign_pin_data_t *data)
{
    if (s_Armed && !data->fired && !data->finished)
    {
        hal_gpio_set_pin_state(data->pin, GPIO_HIGH);

        data->fired = true;
        data->fireTime = hal_time_get_ms_since_boot();

        SYS_LOG("Firing IGN...");
    }
}

static void _ign_update(ign_pin_data_t *data)
{
    if (s_Armed && data->fired && !data->finished)
    {
        if (hal_time_get_ms_since_boot() - data->fireTime >= IGN_UP_TIME_MS)
        {
            hal_gpio_set_pin_state(data->pin, GPIO_LOW);

            data->finished = true;

            SYS_LOG("IGN has been fired!");
        }
    }
}