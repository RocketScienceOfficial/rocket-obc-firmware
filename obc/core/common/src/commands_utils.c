#include "common/commands_utils.h"
#include "drivers/console/console_input.h"
#include "kernel/commands/commands.h"
#include "kernel/commands/default_commands.h"

static INT32 s_ConsoleChar;
static ConsoleInput s_ConsoleInput;
static ConsoleTokens s_ConsoleTokens;

VOID initializeCommands()
{
    registerDefaultConsoleCommands();
}

BOOL consoleAvailable()
{
    return consoleCheckInput(&s_ConsoleChar);
}

VOID checkCommand()
{
    consoleInputProcessCharacter(s_ConsoleChar, &s_ConsoleInput, &s_ConsoleTokens);

    if (s_ConsoleTokens.size > 0)
    {
        CommandArgs args = {0};
        CommandData *command = parseCommand(s_ConsoleTokens.tokens, s_ConsoleTokens.size, &args);

        if (command)
        {
            executeCommand(command, &args);
            commandClearArgs(&args);
        }
        
        consoleInputClear(&s_ConsoleInput, &s_ConsoleTokens);
    }
}