#include "ign.h"
#include "board_config.h"
#include "sm.h"
#include "sensors.h"
#include "ahrs.h"
#include "serial.h"
#include "dataman.h"
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
    hal_msec_t fireTime;
    bool finished;
} ign_pin_data_t;

static ign_pin_data_t s_IGN1;
static ign_pin_data_t s_IGN2;
static ign_pin_data_t s_IGN3;
static ign_pin_data_t s_IGN4;

static void _run_control(void);
static void _run_logic(void);
static void _update_flags(void);
static void _init_pin(ign_pin_data_t *data, hal_pin_number_t pin);
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

uint8_t ign_get_cont_flags(uint8_t ignNumber)
{
    return ignNumber == 1 ? s_IGN1.contFlags : ignNumber == 2 ? s_IGN2.contFlags
                                           : ignNumber == 3   ? s_IGN3.contFlags
                                           : ignNumber == 4   ? s_IGN4.contFlags
                                                              : 0;
}

bool ign_get_state(uint8_t ignNumber)
{
    const ign_pin_data_t *ign = ignNumber == 1 ? &s_IGN1 : ignNumber == 2 ? &s_IGN2
                                                       : ignNumber == 3   ? &s_IGN3
                                                       : ignNumber == 4   ? &s_IGN4
                                                                          : 0;
    return ign->fired && !ign->finished;
}

static void _run_control(void)
{
    if (sm_get_state() == FLIGHT_STATE_STANDING)
    {
        if (events_poll(MSG_CMD_IGN_TEST))
        {
            const char *data = serial_get_param_at_index(0);

            if (data)
            {
                int n = (int)(data[0] - '0');

                SYS_LOG("Testing igniter %d...", n);

                hal_pin_number_t pin = n == 0 ? PIN_IGN_EN_1 : n == 1 ? PIN_IGN_EN_2
                                                           : n == 2   ? PIN_IGN_EN_3
                                                                      : PIN_IGN_EN_4;

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
    if (events_poll(MSG_SM_APOGEE_REACHED))
    {
        _ign_fire(&s_IGN1);

        if (sm_get_apogee() <= dataman_get_config()->mainHeight)
        {
            _ign_fire(&s_IGN2);
        }
    }
    else if (sm_get_state() == FLIGHT_STATE_FREE_FALL)
    {
        if (ahrs_get_data()->position.z - sm_get_base_alt() <= dataman_get_config()->mainHeight)
        {
            _ign_fire(&s_IGN2);
        }
    }

    _ign_update(&s_IGN1);
    _ign_update(&s_IGN2);
    _ign_update(&s_IGN3);
    _ign_update(&s_IGN4);
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
        data->contFlags = IGN_CONT_FLAG_IGN_PRESENT | IGN_CONT_FLAG_FUSE_WORKING;
    }
    else if (v < vref * (IGN_FUSE_WORKING_IGN_NOT_PRESENT_FACTOR + IGN_PIN_CHECK_EPS))
    {
        data->contFlags = IGN_CONT_FLAG_FUSE_WORKING;
    }
    else if (v < vref * (IGN_FUSE_NOT_WORKING_IGN_PRESENT_FACTOR + IGN_PIN_CHECK_EPS))
    {
        data->contFlags = IGN_CONT_FLAG_IGN_PRESENT;
    }
    else if (v < (vref * IGN_FUSE_NOT_WORKING_IGN_NOT_PRESENT_FACTOR + IGN_PIN_CHECK_EPS))
    {
        data->contFlags = 0;
    }
    else
    {
        data->contFlags = 0;
    }
}

static void _ign_fire(ign_pin_data_t *data)
{
    if (!data->fired && !data->finished)
    {
        hal_gpio_set_pin_state(data->pin, GPIO_HIGH);

        data->fired = true;
        data->fireTime = hal_time_get_ms_since_boot();

        SYS_LOG("Firing IGN...");
    }
}

static void _ign_update(ign_pin_data_t *data)
{
    if (data->fired && !data->finished)
    {
        if (hal_time_get_ms_since_boot() - data->fireTime >= IGN_UP_TIME_MS)
        {
            hal_gpio_set_pin_state(data->pin, GPIO_LOW);

            data->finished = true;

            SYS_LOG("IGN has been fired!");
        }
    }
}