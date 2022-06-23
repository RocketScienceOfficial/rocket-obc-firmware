#pragma once

#include "lora.h"
#include <stddef.h>

typedef struct radio_header
{
    size_t paritySize;
    char *parity;
} radio_header_t;

typedef struct radio_body
{
    char command;
    char padding[3];
    size_t payloadSize;
    char *payload;
} radio_body_t;

typedef struct radio_packet
{
    radio_header_t header;
    radio_body_t body;
} radio_packet_t;

void serializeRadioPacket(radio_body_t *body, char **buffer_out_ptr, size_t *size_out);
void deserializeRadioPacket(char *buffer, size_t size, radio_body_t *body_out, int *validationResult);
void radioSendPacket(lora_data_t *lora, radio_body_t *body);
int radioCheckPacket(lora_data_t *lora);
int radioReceivePacket(lora_data_t *lora, radio_body_t *body_out_ptr, int* validationResult_out_ptr);
void radioClearPacket(radio_body_t* body);