#include "pico/stdlib.h"
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
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

    console_input_t input = {0};
    consoleProcessCharacter('h', &input);
    consoleProcessCharacter('e', &input);
    consoleProcessCharacter('l', &input);
    consoleProcessCharacter('l', &input);
    consoleProcessCharacter('o', &input);
    consoleProcessCharacter(' ', &input);
    consoleProcessCharacter('w', &input);
    consoleProcessCharacter('o', &input);
    consoleProcessCharacter('r', &input);
    consoleProcessCharacter('l', &input);
    consoleProcessCharacter('d', &input);
    consoleProcessCharacter('\r', &input);

    MY_ASSERT(input.size == 2);
    MY_ASSERT(strcmp(input.tokens[0], "hello") == 0);
    MY_ASSERT(strcmp(input.tokens[1], "world") == 0);

    consoleClearInput(&input);

    MY_LOG_CORE_INFO("Done!");
}

void loop()
{
    tight_loop_contents();
}