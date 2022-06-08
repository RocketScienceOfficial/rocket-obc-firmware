#include "pico/stdlib.h"
#include <stddef.h>
#include <string.h>
#include "test_framework.h"
#include "logger.h"
#include "log_serial.h"
#include "console_input.h"

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

    char **buffer;
    size_t bufferSize = 0;
    consoleProcessCharacter('h', &buffer, &bufferSize);
    consoleProcessCharacter('e', &buffer, &bufferSize);
    consoleProcessCharacter('l', &buffer, &bufferSize);
    consoleProcessCharacter('l', &buffer, &bufferSize);
    consoleProcessCharacter('o', &buffer, &bufferSize);
    consoleProcessCharacter(' ', &buffer, &bufferSize);
    consoleProcessCharacter('w', &buffer, &bufferSize);
    consoleProcessCharacter('o', &buffer, &bufferSize);
    consoleProcessCharacter('r', &buffer, &bufferSize);
    consoleProcessCharacter('l', &buffer, &bufferSize);
    consoleProcessCharacter('d', &buffer, &bufferSize);
    consoleProcessCharacter('\r', &buffer, &bufferSize);

    MY_ASSERT(bufferSize == 2);
    MY_ASSERT(strcmp(buffer[0], "hello") == 0);
    MY_ASSERT(strcmp(buffer[1], "world") == 0);

    for (int i = 0; i < bufferSize; i++)
    {
        free(buffer[i]);
    }

    free(buffer);

    MY_LOG_CORE_INFO("Done!");
}

void loop()
{
    tight_loop_contents();
}