#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "pico/stdlib.h"
#include "logging/logger.h"
#include "logging/log_serial.h"
#include "utils/time_tracker.h"

static Timer s_Timer;

int main()
{
    const uint LED_PIN = PICO_DEFAULT_LED_PIN;
    gpio_init(LED_PIN);
    gpio_set_dir(LED_PIN, GPIO_OUT);
    gpio_put(LED_PIN, 1);
    sleep_ms(250);

    stdio_init_all();
    sleep_ms(5000);

    myLogCreateConsoleSink(myLogGetCoreLogger(), DEFAULT_LOG_SERIAL_PATTERN);

    while (true)
    {
        if (runEvery(1000, &s_Timer))
        {
            MY_LOG_CORE_INFO("HELLO!");
        }
    }
}