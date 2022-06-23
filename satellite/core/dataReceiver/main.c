#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "pico/stdlib.h"
#include "pinout.h"
#include "logger.h"
#include "log_serial.h"
#include "mg995.h"
#include "console_input.h"
#include "console_commands.h"
#include "remote_commands.h"
#include "default_console_commands.h"
#include "time_tracker.h"
#include "lora.h"
#include "radio_protocol.h"
#include "bmp280.h"
#include "radio_config.h"

static lora_data_t s_LoraData;
static radio_body_t s_PacketBody;
static int s_RadioPacketValidation;
static int s_ConsoleChar;
static console_input_t s_ConsoleInput;

void start();
void initialize();
void loop();
void checkCommand();

int main()
{
    start();

    initialize();

    while (1)
    {
        loop();
    }

    return 0;
}

void start()
{
    stdio_init_all();
    sleep_ms(5000);
}

void initialize()
{
    attachSerialToLog(myLogGetCoreLogger());
    attachSerialToLog(myLogGetMeasureLogger());
    attachSerialToLog(myLogGetCommandOutputLogger());
    attachSerialToLog(myLogGetRemoteCommandLogger());

    MY_LOG_CORE_INFO("Initializing...");

    registerDefaultConsoleCommands();

    lora_pinout_t loraPinout = {
        .spi = SX1278_SPI,
        .sck = SX1278_SCK_GPIO,
        .miso = SX1278_MISO_GPIO,
        .mosi = SX1278_MOSI_GPIO,
        .cs = SX1278_CS_GPIO,
        .ss = SX1278_SS_GPIO,
        .reset = SX1278_RESET_GPIO,
        .dio0 = SX1278_DIO0_GPIO};

    loraInit(&s_LoraData, &loraPinout);

    s_LoraData.pinout = loraPinout;
    s_LoraData.txPower = RADIO_DBM;

    loraBegin(&s_LoraData, RADIO_FREQUENCY_HZ);

    loraSetSpreadingFactor(&s_LoraData, RADIO_SPREADING_FACTOR);
    loraSetSignalBandwidth(&s_LoraData, RADIO_SIGNAL_BANDWIDTH);

    MY_LOG_CORE_INFO("Everything is ready!");
}

void loop()
{
    checkCommand();

    if (radioCheckPacket(&s_LoraData))
    {
        if (radioReceivePacket(&s_LoraData, &s_PacketBody, &s_RadioPacketValidation))
        {
            MY_LOG_CORE_INFO("Packet received!");

            if (s_RadioPacketValidation)
            {
                MY_LOG_CORE_INFO("Packet is valid!");

                bmp280_data_t bmp280Data;
                memcpy(&bmp280Data, s_PacketBody.payload, s_PacketBody.payloadSize);

                MY_LOG_MEASURE_RECEIVER_INT("Pressure", bmp280Data.pressure);
                MY_LOG_MEASURE_RECEIVER_FLOAT("Altitude", bmp280GetAltitude(&bmp280Data));
                MY_LOG_MEASURE_RECEIVER_FLOAT("Temperature", bmp280Data.temperature);

                radioClearPacket(&s_PacketBody);
            }
            else
            {
                MY_LOG_CORE_ERROR("Validation failed!");
            }

            MY_LOG_MEASURE_RECEIVER_INT("RSSI", loraRssi(&s_LoraData));
            MY_LOG_CORE_INFO("Packet processed!");
        }
    }
}

void checkCommand()
{
    s_ConsoleChar = consoleCheckInput();

    if (s_ConsoleChar)
    {
        consoleGetInput(s_ConsoleChar, &s_ConsoleInput);

        if (s_ConsoleInput.size > 0)
        {
            command_args_t args = {0};
            console_command_t *command = parseCommand(s_ConsoleInput.tokens, s_ConsoleInput.size, &args);

            if (command)
            {
                executeCommand(command, &args);
            }

            commandClearArgs(&args);
        }

        consoleClearInput(&s_ConsoleInput);
    }
}