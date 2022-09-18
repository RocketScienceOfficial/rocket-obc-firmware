#pragma once

#include "drivers/lora/sx1278_driver.h"
#include "kernel/services/radio_protocol.h"
#include <stddef.h>

#define RADIO_DBM 20
#define RADIO_SPREADING_FACTOR 10
#define RADIO_SIGNAL_BANDWIDTH 125E3
#define RADIO_FREQUENCY_HZ 433E6

#define MEASUREMENTS_RADIO_COMMAND_ID 'M'
#define COMMANDS_RADIO_COMMAND_ID 'C'

typedef struct RadioUtilPacketData
{
    RadioBody body;
    int signalStrength;
} RadioUtilPacketData;

void initializeRadio(SX1278Pinout *pinout);
bool checkRadioPacket(RadioUtilPacketData* packet);
void sendRadioPacket(RadioBody *body);
void sendRadioRemoteCommand(char *msg);
void radioRemoteCommandCallback(uint8_t *msg, size_t size);