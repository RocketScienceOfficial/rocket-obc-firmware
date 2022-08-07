#include "commands_utils.h"
#include "console_input.h"
#include "commands.h"
#include "default_commands.h"

static int s_ConsoleChar;
static ConsoleInput s_ConsoleInput;
static ConsoleTokens s_ConsoleTokens;

void initializeCommands()
{
    registerDefaultConsoleCommands();
}

bool consoleAvailable()
{
    s_ConsoleChar = consoleCheckInput();

    return s_ConsoleChar != 0;
}

void checkCommand()
{
    consoleProcessCharacter(s_ConsoleChar, &s_ConsoleInput, &s_ConsoleTokens);

    if (s_ConsoleTokens.size > 0)
    {
        CommandArgs args = {0};
        CommandData *command = parseCommand(s_ConsoleTokens.tokens, s_ConsoleTokens.size, &args);

        if (command)
        {
            executeCommand(command, &args);
            commandClearArgs(&args);
        }
        
        consoleClear(&s_ConsoleInput, &s_ConsoleTokens);
    }
}