#pragma once

#include "lora.h"

#define MAX_PAYLOAD_SIZE 64
#define PACKET_KEY '%'

typedef struct radio_header
{
    char command;
    char padding[3];
} radio_header_t;

typedef union radio_packet
{
    struct
    {
        char buffer[sizeof(radio_header_t) + sizeof(int) + sizeof(char) * MAX_PAYLOAD_SIZE];
    };

    struct
    {
        radio_header_t header;
        int payloadSize;
        char payload[MAX_PAYLOAD_SIZE];
    };
} radio_packet_t;

void sendPacket(lora_data_t *lora, radio_packet_t *packet);
int receivePacket(lora_data_t *lora, radio_packet_t *packet);