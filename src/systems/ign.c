#include "ign.h"
#include "sm.h"
#include "sensors.h"
#include "ahrs.h"
#include "serial.h"
#include "radio.h"
#include "dataman.h"
#include "board_config.h"
#include "../middleware/events.h"
#include <hal/gpio_driver.h>
#include <hal/time_tracker.h>
#include <string.h>

#define IGN_UP_TIME_MS 10
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
static uint8_t s_TestedIgniters;

static void _run_control(void);
static void _run_logic(void);
static void _update_flags(void);
static void _init_pin(ign_pin_data_t *data, hal_pin_number_t pin);
static void _set_cont_flags(ign_pin_data_t *data, float v);
static void _ign_fire(ign_pin_data_t *data);
static void _ign_test(uint8_t ignId);
static void _ign_update(ign_pin_data_t *data);

void ign_init(void)
{
    _init_pin(&s_IGN1, PIN_IGN_EN_1);
    _init_pin(&s_IGN2, PIN_IGN_EN_2);
    _init_pin(&s_IGN3, PIN_IGN_EN_3);
    _init_pin(&s_IGN4, PIN_IGN_EN_4);

    SERIAL_DEBUG_LOG("READY");
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

bool ign_is_fired(uint8_t ignNumber)
{
    return ignNumber == 1 ? s_IGN1.fired : ignNumber == 2 ? s_IGN2.fired
                                       : ignNumber == 3   ? s_IGN3.fired
                                       : ignNumber == 4   ? s_IGN4.fired
                                                          : false;
}

bool ign_is_tested(uint8_t ignNumber)
{
    uint8_t flag = 1 << (ignNumber - 1);

    return (s_TestedIgniters & flag) > 0;
}

static void _run_control(void)
{
    if (sm_get_state() == FLIGHT_STATE_STANDING)
    {
        if (events_poll(MSG_SERIAL_MESSAGE_RECEIVED))
        {
            const datalink_frame_structure_serial_t *msg = serial_get_current_message();

            if (msg && msg->msgId == DATALINK_MESSAGE_IGN_REQUEST_TEST)
            {
                const datalink_frame_ign_request_test_t *payload = (const datalink_frame_ign_request_test_t *)msg->payload;

                SERIAL_DEBUG_LOG("Testing igniter %d...", payload->ignNum);

                _ign_test(payload->ignNum);

                datalink_frame_structure_serial_t response = {
                    .msgId = DATALINK_MESSAGE_IGN_FINISH_TEST,
                    .len = 0,
                };
                serial_send_message(&response);
            }
        }
        if (events_poll(MSG_RADIO_PACKET_RECEIVED))
        {
            const datalink_frame_structure_serial_t *msg = radio_get_current_message();

            if (msg && msg->msgId == DATALINK_MESSAGE_TELEMETRY_RESPONSE)
            {
                const datalink_frame_telemetry_response_t *frame = (const datalink_frame_telemetry_response_t *)msg->payload;

                for (uint8_t n = 0; n < 4; n++)
                {
                    uint8_t reqFlag = DATALINK_FLAGS_TELEMETRY_DATA_CONTROL_IGN_1_REQ_FIRE << n;

                    if (frame->flags & reqFlag && !ign_is_tested(n + 1))
                    {
                        _ign_test(n);
                    }
                }
            }
        }
    }
}

static void _run_logic(void)
{
    if (sm_is_armed())
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
            if (!s_IGN2.fired)
            {
                if (vec3_mag_compare(&ahrs_get_data()->velocity, dataman_get_config()->malfunctionSpeed) >= 0)
                {
                    _ign_fire(&s_IGN2);
                }
                else if (sm_get_relative_alt() <= dataman_get_config()->mainHeight)
                {
                    _ign_fire(&s_IGN2);
                }
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
    if (!sm_is_armed())
    {
        return;
    }

    if (!data->fired && !data->finished)
    {
        hal_gpio_set_pin_state(data->pin, GPIO_HIGH);

        data->fired = true;
        data->fireTime = hal_time_get_ms_since_boot();

        SERIAL_DEBUG_LOG("Firing IGN...");
    }
}

static void _ign_test(uint8_t ignId)
{
    hal_pin_number_t pin = ignId == 0 ? PIN_IGN_EN_1 : ignId == 1 ? PIN_IGN_EN_2
                                                   : ignId == 2   ? PIN_IGN_EN_3
                                                                  : PIN_IGN_EN_4;

    hal_gpio_set_pin_state(pin, GPIO_HIGH);
    hal_time_sleep_ms(IGN_UP_TIME_MS);
    hal_gpio_set_pin_state(pin, GPIO_LOW);

    s_TestedIgniters |= (1 << ignId);
}

static void _ign_update(ign_pin_data_t *data)
{
    if (!sm_is_armed())
    {
        return;
    }

    if (data->fired && !data->finished)
    {
        if (hal_time_get_ms_since_boot() - data->fireTime >= IGN_UP_TIME_MS)
        {
            hal_gpio_set_pin_state(data->pin, GPIO_LOW);

            data->finished = true;

            SERIAL_DEBUG_LOG("IGN has been fired!");
        }
    }
}