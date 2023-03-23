#include "drivers/led/w2812_driver.h"
#include "pico/stdlib.h"
#include "hardware/pio.h"
#include "hardware/clocks.h"
#include "ws2812_driver.pio.h"

#define WS2812_FREQ 800000
#define WS2812_PIO pio0
#define WS2812_SM 0

FUNCRESULT ws2812Init(PinNumber pin, BOOL rgbw)
{
    UINT32 offset = pio_add_program(WS2812_PIO, &ws2812_program);

    ws2812_program_init(WS2812_PIO, WS2812_SM, offset, pin, WS2812_FREQ, rgbw);

    return SUC_OK;
}

FUNCRESULT ws2812SetPixel(UINT32 color)
{
    pio_sm_put_blocking(WS2812_PIO, WS2812_SM, color << 8u);

    return SUC_OK;
}