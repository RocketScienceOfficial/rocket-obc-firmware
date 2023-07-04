#include "test_console.h"
#include "drivers/console/console_input.h"
#include <string.h>

MY_TEST_FUNC(CONSOLE_TEST_NAME, 1)
{
    ConsoleInput input = {0};
    BOOL tokensReady = FALSE;

    MY_ASSERT(FUNCSUCCESS(consoleInputProcessCharacter('h', &input, &tokensReady)));
    MY_ASSERT(FUNCSUCCESS(consoleInputProcessCharacter('e', &input, &tokensReady)));
    MY_ASSERT(FUNCSUCCESS(consoleInputProcessCharacter('l', &input, &tokensReady)));
    MY_ASSERT(FUNCSUCCESS(consoleInputProcessCharacter('l', &input, &tokensReady)));
    MY_ASSERT(FUNCSUCCESS(consoleInputProcessCharacter('o', &input, &tokensReady)));
    MY_ASSERT(FUNCSUCCESS(consoleInputProcessCharacter(' ', &input, &tokensReady)));
    MY_ASSERT(FUNCSUCCESS(consoleInputProcessCharacter('w', &input, &tokensReady)));
    MY_ASSERT(FUNCSUCCESS(consoleInputProcessCharacter('o', &input, &tokensReady)));
    MY_ASSERT(FUNCSUCCESS(consoleInputProcessCharacter('r', &input, &tokensReady)));
    MY_ASSERT(FUNCSUCCESS(consoleInputProcessCharacter('l', &input, &tokensReady)));
    MY_ASSERT(FUNCSUCCESS(consoleInputProcessCharacter('d', &input, &tokensReady)));
    MY_ASSERT(FUNCSUCCESS(consoleInputProcessCharacter('\r', &input, &tokensReady)));
    MY_ASSERT(input.tokensSize == 2);
    MY_ASSERT(strcmp(input.tokens[0], "hello") == 0);
    MY_ASSERT(strcmp(input.tokens[1], "world") == 0);

    MY_TEST_END();
}

MY_TEST_FUNC(CONSOLE_TEST_NAME, 2)
{
    ConsoleInput input = {
        .cmd = "hello world",
        .cmdSize = 11,
    };

    MY_ASSERT(__consoleTokenizeInput(&input) == TRUE);
    MY_ASSERT(input.tokensSize == 2);

    MY_TEST_END();
}

MY_TEST_FUNC(CONSOLE_TEST_NAME, 3)
{
    MY_ASSERT(FUNCSUCCESS(consoleInputProcessCharacter(0, NULL, NULL) == FALSE));
    MY_ASSERT(FUNCSUCCESS(__consoleTokenizeInput(NULL) == FALSE));

    MY_TEST_END();
}