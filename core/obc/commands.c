#include "commands.h"
#include "config.h"
#include "modules/drivers/hal/board_control.h"
#include "modules/drivers/hal/serial_driver.h"
#include "modules/drivers/hal/time_tracker.h"
#include "modules/database/dataman.h"
#include <stddef.h>
#include <string.h>

#define CMD(x) hal_serial_printf("CMD:%s\n", x);

bool command_check(void)
{
    msec_t timeOffset = hal_time_get_ms_since_boot();
    int chr = 0;
    char cmdBuffer[512];
    size_t currentSize = 0;

    while (hal_time_get_ms_since_boot() - timeOffset < COMMAND_START_TIME_MS)
    {
        if (hal_serial_read_char(&chr) && chr <= 127)
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
        CMD("data-start");
        dataman_read();
        CMD("data-end");

        return false;
    }

    return true;
}