#include "test_console.h"
#include "console/console_input.h"
#include <string.h>
#include <stddef.h>

static int s_ConsoleChar;
static ConsoleInput s_ConsoleInput;
static ConsoleTokens s_ConsoleTokens;

MY_TEST_FUNC(CONSOLE_TEST_NAME, 1)
{
    ConsoleInput input = {0};
    ConsoleTokens tokens = {0};

    MY_ASSERT(consoleProcessCharacter('h', &input, &tokens));
    MY_ASSERT(consoleProcessCharacter('e', &input, &tokens));
    MY_ASSERT(consoleProcessCharacter('l', &input, &tokens));
    MY_ASSERT(consoleProcessCharacter('l', &input, &tokens));
    MY_ASSERT(consoleProcessCharacter('o', &input, &tokens));
    MY_ASSERT(consoleProcessCharacter(' ', &input, &tokens));
    MY_ASSERT(consoleProcessCharacter('w', &input, &tokens));
    MY_ASSERT(consoleProcessCharacter('o', &input, &tokens));
    MY_ASSERT(consoleProcessCharacter('r', &input, &tokens));
    MY_ASSERT(consoleProcessCharacter('l', &input, &tokens));
    MY_ASSERT(consoleProcessCharacter('d', &input, &tokens));
    MY_ASSERT(consoleProcessCharacter('\r', &input, &tokens));
    MY_ASSERT(tokens.size == 2);
    MY_ASSERT(strcmp(tokens.tokens[0], "hello") == 0);
    MY_ASSERT(strcmp(tokens.tokens[1], "world") == 0);
    MY_ASSERT(consoleClear(&input, &tokens));

    MY_TEST_END();
}

MY_TEST_FUNC(CONSOLE_TEST_NAME, 2)
{
    ConsoleInput input = {
        ._cmd = "hello world",
        ._cmdSize = 11,
    };
    ConsoleTokens tokens = {0};

    MY_ASSERT(consoleTokenizeInput(&input, &tokens));
    MY_ASSERT(tokens.size == 2);

    MY_TEST_END();
}

MY_TEST_FUNC(CONSOLE_TEST_NAME, 3)
{
    MY_ASSERT(consoleProcessCharacter(0, NULL, NULL) == false);
    MY_ASSERT(consoleTokenizeInput(NULL, NULL) == false);
    MY_ASSERT(consoleClear(NULL, NULL) == false);

    MY_TEST_END();
}

MY_TEST_FUNC_DYNAMIC(CONSOLE_TEST_NAME, 1)
{
    s_ConsoleChar = consoleCheckInput();

    if (s_ConsoleChar)
    {
        MY_ASSERT(consoleProcessCharacter(s_ConsoleChar, &s_ConsoleInput, &s_ConsoleTokens));

        if (s_ConsoleTokens.size > 0)
        {
            for (size_t i = 0; i < s_ConsoleTokens.size; i++)
            {
                MY_LOG_CORE_INFO("%s", s_ConsoleTokens.tokens[i]);
            }
        }

        MY_ASSERT(consoleClear(&s_ConsoleInput, &s_ConsoleTokens));
    }

    MY_TEST_END();
}