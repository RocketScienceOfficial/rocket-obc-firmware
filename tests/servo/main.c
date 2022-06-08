#include "pico/stdlib.h"
#include "pinout.h"
#include "logger.h"
#include "log_serial.h"
#include "mg995.h"
#include "test_framework.h"

static unsigned int s_TimerOffset;

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

    mg995_data_t data = {.pin = MG995_PIN};

    mg995Init(&data);

    MY_LOG_CORE_INFO("Everything is ready!");
}

void loop()
{
    tight_loop_contents();
}