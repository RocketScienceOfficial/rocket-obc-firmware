#include "modules/drivers/hal/board_control.h"
#include "pico/stdlib.h"

void hal_board_init(msec_t sleepTimeMs)
{
    hal_time_sleep_ms(1000);

    stdio_init_all();

    if (sleepTimeMs > 0)
    {
        hal_time_sleep_ms(sleepTimeMs);
    }
}

void hal_tight_loop(void)
{
    tight_loop_contents();
}