#include "lib/drivers/led/w2812_driver.h"

#ifdef OBC_PLATFORM_RP2040
#include "pico/stdlib.h"
#include "hardware/pio.h"
#include "hardware/clocks.h"
#include "ws2812_driver_pico.pio.h"

#define WS2812_FREQ 800000
#define WS2812_PIO pio0
#define WS2812_SM 0
#endif

static void _ws2812_send_color(WS2812COLOR color);

void ws2812_init(hal_pin_number_t pin, bool rgbw)
{
#ifdef OBC_PLATFORM_RP2040
    unsigned int offset = pio_add_program(WS2812_PIO, &ws2812_program);

    ws2812_program_init(WS2812_PIO, WS2812_SM, offset, pin, WS2812_FREQ, rgbw);
#endif
}

void ws2812_set_colors(WS2812COLOR *colors, size_t count)
{
    for (size_t i = 0; i < count; i++)
    {
        _ws2812_send_color(colors[i]);
    }
}

WS2812COLOR ws2812_get_color(uint8_t r, uint8_t g, uint8_t b)
{
    return ((unsigned int)r << 8) | ((unsigned int)g << 16) | (unsigned int)b;
}

static void _ws2812_send_color(WS2812COLOR color)
{
#ifdef OBC_PLATFORM_RP2040
    pio_sm_put_blocking(WS2812_PIO, WS2812_SM, color << 8u);
#endif
}