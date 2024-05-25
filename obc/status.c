#include "status.h"
#include "board_config.h"
#include "sensors.h"
#include "ign.h"
#include "lib/drivers/led/w2812_driver.h"

static ws2812_color_t _get_ign_diode_color(hal_voltage_level_t v);
static ws2812_color_t _get_bat_diode_color(uint8_t percent);

void status_init(void)
{
    ws2812_init(PIN_LED, false);
}

void status_update(void)
{
    if (sensors_get_info()->adc)
    {
        ws2812_color_t colors[] = {
            _get_ign_diode_color(sensors_get_frame()->ignDet1),
            _get_ign_diode_color(sensors_get_frame()->ignDet2),
            _get_ign_diode_color(sensors_get_frame()->ignDet3),
            _get_ign_diode_color(sensors_get_frame()->ignDet4),
            ws2812_get_color(0, 255, 0),
            _get_bat_diode_color(sensors_get_frame()->batPercent),
            ws2812_get_color(0, 255, 0),
        };

        ws2812_set_colors(colors, sizeof(colors) / sizeof(ws2812_color_t));
    }
}

static ws2812_color_t _get_ign_diode_color(hal_voltage_level_t v)
{
    ign_pin_state_t state = ign_check_pin(v);

    return state.fuseWorking && state.ignPresent ? ws2812_get_color(0, 255, 0) : !state.fuseWorking || !state.ignPresent ? ws2812_get_color(255, 255, 0)
                                                                             : !state.fuseWorking && !state.ignPresent   ? ws2812_get_color(255, 0, 0)
                                                                                                                         : ws2812_get_color(0, 0, 0);
}

static ws2812_color_t _get_bat_diode_color(uint8_t percent)
{
    if (percent < 50)
    {
        return ws2812_get_color(255, 255 * percent / 50, 0);
    }
    else
    {
        return ws2812_get_color(255 - 255 * (percent - 50) / 50, 255, 0);
    }
}