#include "commands.h"
#include "config.h"
#include "modules/drivers/hal/board_control.h"
#include "modules/drivers/hal/serial_driver.h"
#include "modules/drivers/hal/time_tracker.h"
#include "modules/database/dataman.h"
#include <stddef.h>
#include <string.h>

static void _clear_input_buffer(void);

bool command_check(void)
{
    msec_t timeOffset = hal_time_get_ms_since_boot();
    int chr = 0;
    char cmdBuffer[512];
    size_t currentSize = 0;

    _clear_input_buffer();

    while (hal_time_get_ms_since_boot() - timeOffset < COMMAND_START_TIME_MS)
    {
        if (hal_serial_read_char(&chr))
        {
            if (chr != '\r')
            {
                cmdBuffer[currentSize++] = chr;
            }
            else
            {
                cmdBuffer[currentSize] = '\0';

                break;
            }
        }
    }

    if (strcmp(cmdBuffer, "start-download") == 0)
    {
        dataman_read();

        return false;
    }

    return true;
}

static void _clear_input_buffer(void)
{
    int chr = 0;

    while (hal_serial_read_char(&chr))
    {
    }
}