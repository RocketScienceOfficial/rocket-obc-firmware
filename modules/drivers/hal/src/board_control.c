#include "modules/drivers/hal/board_control.h"
#include "pico/stdlib.h"

void board_init(time_t sleepTimeMS)
{
    time_sleep_ms(1000);
    
    stdio_init_all();

    if (sleepTimeMS > 0)
    {
        time_sleep_ms(sleepTimeMS);
    }
}

void tight_loop(void)
{
    tight_loop_contents();
}