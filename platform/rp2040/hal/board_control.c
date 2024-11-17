#include "hal/board_control.h"
#include "pico/stdlib.h"

bool hal_board_init(hal_msec_t sleepTimeMs)
{
    hal_time_sleep_ms(1000);

    stdio_init_all();
    stdio_set_translate_crlf(&stdio_usb, false);

    if (sleepTimeMs > 0)
    {
        hal_time_sleep_ms(sleepTimeMs);
    }

    return true;
}

void hal_board_tight_loop(void)
{
    tight_loop_contents();
}

bool hal_board_is_usb_connected(void)
{
    return stdio_usb_connected();
}