#include "test_console.h"
#include "drivers/console/console_input.h"
#include <string.h>

static UINT32 s_ConsoleChar;
static ConsoleInput s_ConsoleInput;
static ConsoleTokens s_ConsoleTokens;

MY_TEST_FUNC(CONSOLE_TEST_NAME, 1)
{
    ConsoleInput input = {0};
    ConsoleTokens tokens = {0};

    MY_ASSERT(FUNCSUCCESS(consoleInputProcessCharacter('h', &input, &tokens)));
    MY_ASSERT(FUNCSUCCESS(consoleInputProcessCharacter('e', &input, &tokens)));
    MY_ASSERT(FUNCSUCCESS(consoleInputProcessCharacter('l', &input, &tokens)));
    MY_ASSERT(FUNCSUCCESS(consoleInputProcessCharacter('l', &input, &tokens)));
    MY_ASSERT(FUNCSUCCESS(consoleInputProcessCharacter('o', &input, &tokens)));
    MY_ASSERT(FUNCSUCCESS(consoleInputProcessCharacter(' ', &input, &tokens)));
    MY_ASSERT(FUNCSUCCESS(consoleInputProcessCharacter('w', &input, &tokens)));
    MY_ASSERT(FUNCSUCCESS(consoleInputProcessCharacter('o', &input, &tokens)));
    MY_ASSERT(FUNCSUCCESS(consoleInputProcessCharacter('r', &input, &tokens)));
    MY_ASSERT(FUNCSUCCESS(consoleInputProcessCharacter('l', &input, &tokens)));
    MY_ASSERT(FUNCSUCCESS(consoleInputProcessCharacter('d', &input, &tokens)));
    MY_ASSERT(FUNCSUCCESS(consoleInputProcessCharacter('\r', &input, &tokens)));
    MY_ASSERT(tokens.size == 2);
    MY_ASSERT(strcmp(tokens.tokens[0], "hello") == 0);
    MY_ASSERT(strcmp(tokens.tokens[1], "world") == 0);
    MY_ASSERT(FUNCSUCCESS(consoleInputClear(&input, &tokens)));

    MY_TEST_END();
}

MY_TEST_FUNC(CONSOLE_TEST_NAME, 2)
{
    ConsoleInput input = {
        ._cmd = "hello world",
        ._cmdSize = 11,
    };
    ConsoleTokens tokens = {0};

    MY_ASSERT(FUNCSUCCESS(consoleTokenizeInput(&input, &tokens)));
    MY_ASSERT(tokens.size == 2);

    MY_TEST_END();
}

MY_TEST_FUNC(CONSOLE_TEST_NAME, 3)
{
    MY_ASSERT(FUNCSUCCESS(consoleInputProcessCharacter(0, NULL, NULL) == FALSE));
    MY_ASSERT(FUNCSUCCESS(consoleTokenizeInput(NULL, NULL) == FALSE));
    MY_ASSERT(FUNCSUCCESS(consoleInputClear(NULL, NULL) == FALSE));

    MY_TEST_END();
}