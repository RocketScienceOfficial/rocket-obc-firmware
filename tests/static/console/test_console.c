#include "test_console.h"
#include "console_input.h"
#include <string.h>

MY_TEST_FUNC(CONSOLE_TEST_NAME, 1)
{
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

    MY_TEST_END();
}