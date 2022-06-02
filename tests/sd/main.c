#include <string.h>
#include "pico/stdlib.h"
#include "pinout.h"
#include "sd.h"
#include "my_assert.h"
#include "logger.h"
#include "log_serial.h"
#include "time_tracker.h"

#define TEST_FILENAME "test.txt"

static unsigned int s_TimerOffset = 0;
static char s_Buffer[512];

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

    sdInit();
    sdInitFile(TEST_FILENAME);
    sdFlush(TEST_FILENAME);
    sdAttachToCoreLogger(TEST_FILENAME);

    sdBegin(TEST_FILENAME);

    MY_LOG_CORE_INFO("Everything is ready!");

    sdEnd(TEST_FILENAME);
}

void loop()
{
    if (runEvery(1000, &s_TimerOffset))
    {
        sdBegin(TEST_FILENAME);

        MY_LOG_CORE_INFO("Hello world!");

        sdEnd(TEST_FILENAME);
    }
}