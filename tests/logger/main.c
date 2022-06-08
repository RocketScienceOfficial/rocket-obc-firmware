#include "pico/stdlib.h"
#include "test_framework.h"
#include "logger.h"
#include "log_serial.h"

void start();
void initialize();
void loop();

int main()
{
    start();
    initialize();

    while (1)
    {
        loop();
    }

    return 0;
}

void start()
{
    stdio_init_all();
    sleep_ms(5000);
}

void initialize()
{
    attachSerialToLog(myLogGetCoreLogger());

    MY_LOG_CORE_INFO("Initializing...");
    MY_LOG_CORE_INFO("TEST NUMBER: %d", 123);
    MY_LOG_CORE_INFO("Done!");
}

void loop()
{
    tight_loop_contents();
}