#include "modules/drivers/hal/board_control.h"
#include "pico/stdlib.h"

void hal_board_init(hal_time_t sleepTimeMS)
{
    hal_time_sleep_ms(1000);
    
    stdio_init_all();

    if (sleepTimeMS > 0)
    {
        hal_time_sleep_ms(sleepTimeMS);
    }
}

void hal_tight_loop(void)
{
    tight_loop_contents();
}