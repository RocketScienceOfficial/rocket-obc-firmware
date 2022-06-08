#include "pico/stdlib.h"
#include "test_framework.h"
#include "logger.h"
#include "log_serial.h"
#include "time_tracker.h"

static unsigned int s_TimerOffset = 0;

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

    MY_ASSERT(getMsSinceBoot() >= 5000);

    MY_LOG_CORE_INFO("Done!");
}

void loop()
{
    tight_loop_contents();

    if (runEvery(1000, &s_TimerOffset))
    {
        MY_LOG_CORE_INFO("Hello!");
    }
}