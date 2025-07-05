#include "status.h"
#include "sensors.h"
#include "dataman.h"
#include "ign.h"
#include "sm.h"
#include "serial.h"
#include "board_config.h"
#include "../middleware/events.h"
#include <hal/pwm_driver.h>
#include <lib/drivers/led/w2812_driver.h>
#include <stdbool.h>

#define WS_BRIGHTNESS 0.05f
#define WS_COLOR(r, g, b) ws2812_get_color((uint8_t)((r) * WS_BRIGHTNESS), (uint8_t)((g) * WS_BRIGHTNESS), (uint8_t)((b) * WS_BRIGHTNESS))
#define OTHER_DIODES_UPDATE_PERIOD_MS 500
#define BUZZER_FREQ 2730
#define BUZZER_DUTY_CYCLE_US (1e6f / BUZZER_FREQ * 0.5f)

typedef struct buzzer_tone
{
    uint16_t frequency;
    uint16_t duration_ms;
} buzzer_tone_t;

static ws2812_color_t s_Diodes[7];
static hal_pwm_config_t s_BuzzerPWMConfig;
static hal_msec_t s_OtherDiodesTimeOffset;

static const buzzer_tone_t *s_CurrentTone;
static size_t s_CurrentToneSize;
static hal_msec_t s_BuzzerToneStartTime;

static const buzzer_tone_t s_StartupMusic[] = {
    {880, 100},
    {0, 50},
    {1047, 100},
    {0, 50},
    {1319, 100},
};
static const buzzer_tone_t s_ArmMusic[] = {
    {1480, 100},
    {0, 50},
    {1480, 100},
    {0, 50},
    {1480, 100},
};
static const buzzer_tone_t s_DisarmMusic[] = {
    {698, 100},
    {0, 50},
    {698, 100},
    {0, 50},
    {698, 100},
};
static const buzzer_tone_t s_LandedMusic[] = {
    {BUZZER_FREQ, 500},
    {0, 500},
};

static void _init_diodes(void);
static void _handle_diodes(void);
static void _update_diodes_colors(void);
static ws2812_color_t _get_ign_diode_color(uint8_t ignNumber);
static ws2812_color_t _get_arm_diode_color(void);
static ws2812_color_t _get_bat_diode_color(void);
static ws2812_color_t _get_ready_diode_color(void);
static void _init_buzzer(void);
static void _handle_buzzer(void);
static void _play_current_buzzer_tone(void);
static void _set_buzzer_active(bool active);

void status_init(void)
{
    _init_diodes();
    _init_buzzer();

    SERIAL_DEBUG_LOG("READY!");
}

void status_update(void)
{
    _handle_diodes();
    _handle_buzzer();
}

static void _init_diodes(void)
{
    ws2812_init(PIN_LED, false);

    s_Diodes[0] = WS_COLOR(0, 0, 0);
    s_Diodes[1] = WS_COLOR(0, 0, 0);
    s_Diodes[2] = WS_COLOR(0, 0, 0);
    s_Diodes[3] = WS_COLOR(0, 0, 0);
    s_Diodes[4] = WS_COLOR(0, 0, 0);
    s_Diodes[5] = WS_COLOR(0, 0, 0);
    s_Diodes[6] = WS_COLOR(0, 0, 0);

    _update_diodes_colors();
}

static void _handle_diodes(void)
{
    if (events_poll(MSG_SENSORS_ADC_READ))
    {
        s_Diodes[0] = _get_ign_diode_color(1);
        s_Diodes[1] = _get_ign_diode_color(2);
        s_Diodes[2] = _get_ign_diode_color(3);
        s_Diodes[3] = _get_ign_diode_color(4);
        s_Diodes[5] = _get_bat_diode_color();

        _update_diodes_colors();
    }

    if (hal_time_get_ms_since_boot() - s_OtherDiodesTimeOffset >= OTHER_DIODES_UPDATE_PERIOD_MS)
    {
        s_Diodes[4] = _get_ready_diode_color();
        s_Diodes[6] = _get_arm_diode_color();

        _update_diodes_colors();

        s_OtherDiodesTimeOffset = hal_time_get_ms_since_boot();
    }
}

static void _update_diodes_colors(void)
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

static void _init_buzzer(void)
{
    hal_pwm_init_pin(&s_BuzzerPWMConfig, PIN_BUZZER, BUZZER_FREQ);

    s_CurrentTone = s_StartupMusic;
    s_CurrentToneSize = sizeof(s_StartupMusic) / sizeof(buzzer_tone_t);

    _play_current_buzzer_tone();
}

static void _handle_buzzer(void)
{
    if (events_poll(MSG_SM_ARMED))
    {
        s_CurrentTone = s_ArmMusic;
        s_CurrentToneSize = sizeof(s_ArmMusic) / sizeof(buzzer_tone_t);

        _play_current_buzzer_tone();
    }
    else if (events_poll(MSG_SM_DISARMED))
    {
        s_CurrentTone = s_DisarmMusic;
        s_CurrentToneSize = sizeof(s_DisarmMusic) / sizeof(buzzer_tone_t);

        _play_current_buzzer_tone();
    }
    else if (sm_get_state() == FLIGHT_STATE_LANDED)
    {
        if (s_CurrentTone == NULL)
        {
            s_CurrentTone = s_LandedMusic;
            s_CurrentToneSize = sizeof(s_LandedMusic) / sizeof(buzzer_tone_t);

            _play_current_buzzer_tone();
        }
    }

    if (s_CurrentTone != NULL)
    {
        if (hal_time_get_ms_since_boot() - s_BuzzerToneStartTime >= s_CurrentTone->duration_ms)
        {
            if (s_CurrentToneSize > 1)
            {
                s_CurrentTone++;
                s_CurrentToneSize--;
            }
            else
            {
                s_CurrentTone = NULL;
            }

            _play_current_buzzer_tone();
        }
    }
}

static void _play_current_buzzer_tone(void)
{
    if (s_CurrentTone == NULL || s_CurrentTone->frequency == 0)
    {
        _set_buzzer_active(false);
    }
    else
    {
        hal_pwm_set_frequency(&s_BuzzerPWMConfig, s_CurrentTone->frequency);

        _set_buzzer_active(true);
    }

    SERIAL_DEBUG_LOG("Buzzer tone: %d Hz, duration: %d ms", s_CurrentTone ? s_CurrentTone->frequency : 0, s_CurrentTone ? s_CurrentTone->duration_ms : 0);

    s_BuzzerToneStartTime = hal_time_get_ms_since_boot();
}

static void _set_buzzer_active(bool active)
{
    hal_pwm_set_duty(&s_BuzzerPWMConfig, active ? BUZZER_DUTY_CYCLE_US : 0);
}