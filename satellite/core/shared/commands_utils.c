#include "commands_utils.h"
#include "console_input.h"
#include "commands.h"
#include "default_commands.h"

static int s_ConsoleChar;
static ConsoleInput s_ConsoleInput;

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
    consoleProcessCharacter(s_ConsoleChar, &s_ConsoleInput);

    if (s_ConsoleInput.size > 0)
    {
        CommandArgs args = {0};
        CommandData *command = parseCommand(s_ConsoleInput.tokens, s_ConsoleInput.size, &args);

        if (command)
        {
            executeCommand(command, &args);
            commandClearArgs(&args);
        }
        
        consoleClearInput(&s_ConsoleInput);
    }
}