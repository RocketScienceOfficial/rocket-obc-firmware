#include "test_commands.h"
#include "commands/commands.h"
#include "commands/default_commands.h"

MY_TEST_INIT_FUNC(COMMANDS_TEST_NAME)
{
    MY_ASSERT(registerDefaultConsoleCommands());

    MY_TEST_END();
}

MY_TEST_FUNC(COMMANDS_TEST_NAME, 1)
{
    char *tokens[] = {
        "hello",
    };
    size_t tokensSize = 1;
    CommandArgs args;
    CommandData *command = parseCommand(tokens, tokensSize, &args);

    if (command != NULL)
    {
        MY_ASSERT(executeCommand(command, &args));
    }

    MY_ASSERT(commandClearArgs(&args));

    MY_TEST_END();
}

MY_TEST_FUNC(COMMANDS_TEST_NAME, 2)
{
    MY_ASSERT(registerCommand(NULL) == false);
    MY_ASSERT(parseCommand(NULL, 0, NULL) == NULL);
    MY_ASSERT(executeCommand(NULL, NULL) == false);

    MY_TEST_END();
}

MY_TEST_FUNC(COMMANDS_TEST_NAME, 3)
{
    CommandData dummyCommand = {.name = "dummy", .func = &helloCommand};

    MY_ASSERT(COMMANDS_MAX_COUNT - defaultCommandsGetCount() >= 0);

    for (size_t i = 0; i < COMMANDS_MAX_COUNT - defaultCommandsGetCount(); i++)
    {
        MY_ASSERT(registerCommand(&dummyCommand));
    }

    MY_ASSERT(registerCommand(&dummyCommand) == false);

    MY_TEST_END();
}