#include "status.h"
#include "sensors.h"
#include "dataman.h"
#include "ign.h"
#include "sm.h"
#include "serial.h"
#include "board_config.h"
#include "../middleware/events.h"
#include <lib/drivers/led/w2812_driver.h>
#include <lib/drivers/buzzer/passive_buzzer_driver.h>

#define WS_BRIGHTNESS 0.05f
#define WS_COLOR(r, g, b) ws2812_get_color((uint8_t)((r) * WS_BRIGHTNESS), (uint8_t)((g) * WS_BRIGHTNESS), (uint8_t)((b) * WS_BRIGHTNESS))
#define OTHER_DIODES_UPDATE_PERIOD_MS 500
#define BUZZER_FREQ 2730
#define BUZZER_DUTY_CYCLE_US (1e6f / BUZZER_FREQ * 0.5f)
#define BUZZER_DELAY_MS 1000

static ws2812_color_t s_Diodes[7];
static hal_pwm_config_t s_BuzzerConfig;
static hal_msec_t s_OtherDiodesTimeOffset;
static bool s_BuzzerActive;
static hal_msec_t s_BuzzerTimeOffset;

static void _update_diodes(void);
static ws2812_color_t _get_ign_diode_color(uint8_t ignNumber);
static ws2812_color_t _get_arm_diode_color(void);
static ws2812_color_t _get_bat_diode_color(void);
static ws2812_color_t _get_ready_diode_color(void);

void status_init(void)
{
    ws2812_init(PIN_LED, false);

    s_Diodes[0] = WS_COLOR(0, 0, 0);
    s_Diodes[1] = WS_COLOR(0, 0, 0);
    s_Diodes[2] = WS_COLOR(0, 0, 0);
    s_Diodes[3] = WS_COLOR(0, 0, 0);
    s_Diodes[4] = WS_COLOR(0, 0, 0);
    s_Diodes[5] = WS_COLOR(0, 0, 0);
    s_Diodes[6] = WS_COLOR(0, 0, 0);

    _update_diodes();

    passive_buzzer_init(&s_BuzzerConfig, PIN_BUZZER, BUZZER_FREQ);

    SERIAL_DEBUG_LOG("READY!");
}

void status_update(void)
{
    if (events_poll(MSG_SENSORS_ADC_READ))
    {
        s_Diodes[0] = _get_ign_diode_color(1);
        s_Diodes[1] = _get_ign_diode_color(2);
        s_Diodes[2] = _get_ign_diode_color(3);
        s_Diodes[3] = _get_ign_diode_color(4);
        s_Diodes[5] = _get_bat_diode_color();

        _update_diodes();
    }

    if (hal_time_get_ms_since_boot() - s_OtherDiodesTimeOffset >= OTHER_DIODES_UPDATE_PERIOD_MS)
    {
        s_Diodes[4] = _get_ready_diode_color();
        s_Diodes[6] = _get_arm_diode_color();

        _update_diodes();

        s_OtherDiodesTimeOffset = hal_time_get_ms_since_boot();
    }

    if (sm_get_state() == FLIGHT_STATE_LANDED)
    {
        if (hal_time_get_ms_since_boot() - s_BuzzerTimeOffset >= BUZZER_DELAY_MS)
        {
            s_BuzzerActive = !s_BuzzerActive;
            s_BuzzerTimeOffset = hal_time_get_ms_since_boot();

            passive_buzzer_set_active(&s_BuzzerConfig, s_BuzzerActive ? BUZZER_DUTY_CYCLE_US : 0.0f);
        }
    }
}

static void _update_diodes(void)
{
    ws2812_set_colors(s_Diodes, sizeof(s_Diodes) / sizeof(ws2812_color_t));
}

static ws2812_color_t _get_ign_diode_color(uint8_t ignNumber)
{
    uint8_t flags = ign_get_cont_flags(ignNumber);

    if (flags != 0)
    {
        if (flags & IGN_CONT_FLAG_FUSE_WORKING)
        {
            if (flags & IGN_CONT_FLAG_IGN_PRESENT)
            {
                return WS_COLOR(0, 255, 0);
            }
            else
            {
                return WS_COLOR(255, 0, 0);
            }
        }
        else
        {
            return WS_COLOR(255, 165, 0);
        }
    }
    else
    {
        return WS_COLOR(0, 0, 0);
    }
}

static ws2812_color_t _get_arm_diode_color(void)
{
    return sm_is_armed() ? WS_COLOR(0, 255, 0) : WS_COLOR(255, 0, 0);
}

static ws2812_color_t _get_bat_diode_color(void)
{
    uint8_t percent = sensors_get_frame()->batPercent;

    if (percent == 0)
    {
        return WS_COLOR(0, 0, 0);
    }

    if (percent < 50)
    {
        return WS_COLOR(255, 255 * percent / 50, 0);
    }
    else
    {
        return WS_COLOR(255 - 255 * (percent - 50) / 50, 255, 0);
    }
}

static ws2812_color_t _get_ready_diode_color(void)
{
    return dataman_is_ready() ? WS_COLOR(0, 255, 0) : WS_COLOR(255, 0, 0);
}