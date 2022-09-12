#pragma once

#include <stddef.h>
#include "myhardware/radio.h"

#define RADIO_DBM 20
#define RADIO_SPREADING_FACTOR 10
#define RADIO_SIGNAL_BANDWIDTH 125E3
#define RADIO_FREQUENCY_HZ 433E6

#define MEASUREMENTS_RADIO_COMMAND_ID 'M'
#define COMMANDS_RADIO_COMMAND_ID 'C'

void initRadioCommand(LoRaData *data);
void sendRadioRemoteCommand(char *msg);
void radioRemoteCommandCallback(uint8_t *msg, size_t size);