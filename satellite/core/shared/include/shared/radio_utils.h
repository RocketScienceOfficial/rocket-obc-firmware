#pragma once

#include "tools/typedefs.h"
#include "drivers/lora/sx1278_driver.h"
#include "kernel/services/radio_protocol.h"

#define RADIO_DBM 20
#define RADIO_SPREADING_FACTOR 10
#define RADIO_SIGNAL_BANDWIDTH 125E3
#define RADIO_FREQUENCY_HZ 433E6

#define MEASUREMENTS_RADIO_COMMAND_ID 'M'
#define COMMANDS_RADIO_COMMAND_ID 'C'

typedef struct RadioUtilPacketData
{
    RadioBody body;
    INT32 signalStrength;
} RadioUtilPacketData;

void initializeRadio(SX1278Pinout *pinout);
BOOL checkRadioPacket(RadioUtilPacketData *packet);
void sendRadioPacket(RadioBody *body);
void sendRadioRemoteCommand(STRING msg);
void radioRemoteCommandCallback(BYTE *msg, SIZE size);