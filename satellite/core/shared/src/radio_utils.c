#include "shared/radio_utils.h"
#include "shared/hardware_utils.h"
#include "logging/logger.h"
#include "myhardware/radio.h"
#include "myhardware/radio_protocol.h"
#include "console/console_input.h"
#include "commands/commands.h"
#include <string.h>

static LoRaData *s_Lora;

void initRadioCommand(LoRaData *data)
{
    s_Lora = data;
}

void sendRadioRemoteCommand(char *msg)
{
    RadioBody body = {
        .command = COMMANDS_RADIO_COMMAND_ID,
        .payloadSize = strlen(msg),
        .payload = (uint8_t *)msg,
    };

    HW_CALL(radioSendPacket(&s_Lora, &body));
}

void radioRemoteCommandCallback(uint8_t *msg, size_t size)
{
    ConsoleInput input = {0};
    ConsoleTokens tokens = {0};

    for (int i = 0; i < size; i++)
    {
        consoleProcessCharacter(msg[i], &input, &tokens);
    }

    consoleProcessCharacter('\r', &input, &tokens);

    CommandArgs args = {0};
    CommandData *command = parseCommand(tokens.tokens, tokens.size, &args);

    if (command)
    {
        executeCommand(command, &args);
        commandClearArgs(&args);
    }

    consoleClear(&input, &tokens);
}