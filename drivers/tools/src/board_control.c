#include "drivers/tools/board_control.h"
#include "pico/stdlib.h"

VOID boardInit(TIME sleepTimeMS)
{
    stdio_init_all();

    if (sleepTimeMS > 0)
    {
        sleep_ms(sleepTimeMS);
    }
}

VOID tightLoop()
{
    tight_loop_contents();
}