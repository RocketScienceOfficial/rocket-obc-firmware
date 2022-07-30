#include "commands_utils.h"
#include "console_input.h"
#include "console_commands.h"
#include "remote_commands.h"
#include "default_console_commands.h"

static int s_ConsoleChar;
static console_input_t s_ConsoleInput;

void initializeCommands()
{
    registerDefaultConsoleCommands();
}

int consoleAvailable()
{
    s_ConsoleChar = consoleCheckInput();

    return s_ConsoleChar != 0;
}

void checkCommand()
{
    consoleGetInput(s_ConsoleChar, &s_ConsoleInput);

    if (s_ConsoleInput.size > 0)
    {
        command_args_t args = {0};
        console_command_t *command = parseCommand(s_ConsoleInput.tokens, s_ConsoleInput.size, &args);

        if (command)
        {
            executeCommand(command, &args);
            commandClearArgs(&args);
        }
        
        consoleClearInput(&s_ConsoleInput);
    }
}