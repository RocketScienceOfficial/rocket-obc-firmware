#include "test_console.h"
#include "console_input.h"
#include <string.h>
#include <stddef.h>

static int s_ConsoleChar;
static console_input_t s_ConsoleInput;

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

MY_TEST_FUNC_DYNAMIC(CONSOLE_TEST_NAME, 1)
{
    s_ConsoleChar = consoleCheckInput();

    if (s_ConsoleChar)
    {
        consoleGetInput(s_ConsoleChar, &s_ConsoleInput);
        
        if (s_ConsoleInput.size > 0)
        {
            for (size_t i = 0; i < s_ConsoleInput.size; i++)
            {
                MY_LOG_CORE_INFO("%s", s_ConsoleInput.tokens[i]);
            }
        }

        consoleClearInput(&s_ConsoleInput);
    }

    MY_TEST_END();
}