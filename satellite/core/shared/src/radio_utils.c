#include "shared/radio_utils.h"
#include "shared/functions_utils.h"
#include "kernel/logging/logger.h"
#include "kernel/console/console_input.h"
#include "kernel/commands/commands.h"
#include <stdlib.h>
#include <string.h>

static SX1278Data s_LoraData;

void initializeRadio(SX1278Pinout *pinout)
{
    loraInit(&s_LoraData, pinout);
    loraBegin(&s_LoraData, RADIO_FREQUENCY_HZ);
    loraSetTxPower(&s_LoraData, RADIO_DBM);
    loraSetSpreadingFactor(&s_LoraData, RADIO_SPREADING_FACTOR);
    loraSetSignalBandwidth(&s_LoraData, RADIO_SIGNAL_BANDWIDTH);
}

bool checkRadioPacket(RadioUtilPacketData *packet)
{
    size_t packetSize = loraParsePacket(&s_LoraData, 0);

    if (packetSize)
    {
        MY_LOG_CORE_INFO("Packet is available!");

        uint8_t buffer[packetSize];
        size_t i = 0;

        while (loraAvailable(&s_LoraData))
        {
            buffer[i] = (uint8_t)loraRead(&s_LoraData);
            i++;
        }

        packet->body = (RadioBody){0};
        packet->signalStrength = loraRssi(&s_LoraData);

        bool packetValidation = false;

        FUNC_CALL(deserializeRadioPacket(buffer, packetSize, &packet->body, &packetValidation));

        if (packetValidation)
        {
            MY_LOG_CORE_INFO("Packet is valid!");
        }
        else
        {
            MY_LOG_CORE_ERROR("Validation failed!");
        }

        MY_LOG_CORE_INFO("Packet processed!");

        return packetValidation;
    }

    return false;
}

void sendRadioPacket(RadioBody *body)
{
    uint8_t *packetBuffer;
    size_t packetBufferSize = 0;

    FUNC_CALL(serializeRadioPacket(body, &packetBuffer, &packetBufferSize));

    loraBeginPacket(&s_LoraData, 0);
    loraWrite_str_s(&s_LoraData, packetBuffer, packetBufferSize);
    loraEndPacket(&s_LoraData, 0);

    free(packetBuffer);
}

void sendRadioRemoteCommand(char *msg)
{
    RadioBody body = {
        .command = COMMANDS_RADIO_COMMAND_ID,
        .payloadSize = strlen(msg),
        .payload = (uint8_t *)msg,
    };

    sendRadioPacket(&body);
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