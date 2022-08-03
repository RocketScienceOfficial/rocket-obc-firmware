#pragma once

#include "radio.h"
#include <stdint.h>
#include <stddef.h>

typedef struct radio_header
{
    size_t paritySize;
    uint8_t *parity;
} radio_header_t;

typedef struct radio_body
{
    uint8_t command;
    uint8_t padding[3];
    size_t payloadSize;
    uint8_t *payload;
} radio_body_t;

typedef struct radio_packet
{
    radio_header_t header;
    radio_body_t body;
} radio_packet_t;

void serializeRadioPacket(radio_body_t *body, uint8_t **buffer_out_ptr, size_t *size_out);
void deserializeRadioPacket(uint8_t *buffer, size_t size, radio_body_t *body_out, int *validationResult);
void radioSendPacket(lora_data_t *lora, radio_body_t *body);
int radioCheckPacket(lora_data_t *lora);
int radioReceivePacket(lora_data_t *lora, radio_body_t *body_out_ptr, int* validationResult_out_ptr);
void radioClearPacket(radio_body_t* body);