#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "pico/stdlib.h"
#include "kernel/logging/logger.h"
#include "drivers/console/console_output.h"

int main()
{
    stdio_init_all();
    sleep_ms(5000);

    myLogCreateConsoleSink(myLogGetCoreLogger(), DEFAULT_LOG_SERIAL_PATTERN);

    while (TRUE)
    {
        MY_LOG_CORE_INFO("Hello, world!");

        sleep_ms(1000);
    }

    return 0;
}