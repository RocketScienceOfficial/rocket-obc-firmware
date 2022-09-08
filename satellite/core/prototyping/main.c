#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "pico/stdlib.h"
#include "logging/logger.h"
#include "logging/log_serial.h"
#include "utils/time_tracker.h"

static Timer s_TimerOffset;

int main()
{
    stdio_init_all();
    sleep_ms(5000);

    myLogCreateConsoleSink(myLogGetCoreLogger(), DEFAULT_LOG_SERIAL_PATTERN);

    while (true)
    {
        if (runEvery(1000, &s_TimerOffset))
        {
            MY_LOG_CORE_INFO("TEST!");
        }
    }
}