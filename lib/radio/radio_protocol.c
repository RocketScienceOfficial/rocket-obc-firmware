#include "radio_protocol.h"
#include "cryptography.h"
#include <string.h>
#include <stdlib.h>
#include "logger.h"

const char RADIO_PACKET_KEY[] = {
    0x40,
    0x48,
};

void serializeRadioPacket(radio_body_t *body, char **buffer_out_ptr, size_t *size_out)
{
    size_t bodySize = sizeof(char) + sizeof(size_t) + sizeof(char) * body->payloadSize;
    char bodyBuffer[bodySize];

    memcpy(bodyBuffer, &body->command, sizeof(char) + sizeof(size_t));
    memcpy(bodyBuffer + sizeof(char) + sizeof(size_t), body->payload, sizeof(char) * body->payloadSize);

    radio_header_t header = {
        .paritySize = 0,
        .parity = NULL,
    };

    calculateParity(bodyBuffer, bodySize, &header.parity, &header.paritySize);

    size_t headerSize = sizeof(size_t) + sizeof(char) * header.paritySize;
    char headerBuffer[headerSize];

    memcpy(headerBuffer, &header.paritySize, sizeof(size_t));
    memcpy(headerBuffer + sizeof(size_t), header.parity, sizeof(char) * header.paritySize);

    free(header.parity);

    size_t bufferSize = headerSize + bodySize;
    *buffer_out_ptr = (char *)malloc(bufferSize);
    *size_out = bufferSize;

    memcpy(*buffer_out_ptr, headerBuffer, headerSize);
    memcpy(*buffer_out_ptr + headerSize, bodyBuffer, bodySize);
}

void deserializeRadioPacket(char *buffer, size_t size, radio_body_t *body_out, int *validationResult)
{
    radio_packet_t packet = {
        .header = {0},
        .body = {0},
    };

    size_t currentBufferOffset = 0;
    memcpy(&packet.header, buffer + currentBufferOffset, sizeof(size_t));

    currentBufferOffset += sizeof(size_t);
    packet.header.parity = (char *)malloc(sizeof(char) * packet.header.paritySize);
    memcpy(packet.header.parity, buffer + currentBufferOffset, packet.header.paritySize * sizeof(char));

    currentBufferOffset += packet.header.paritySize * sizeof(char);
    size_t bodyOffset = currentBufferOffset;
    memcpy(&packet.body, buffer + currentBufferOffset, sizeof(char) + sizeof(size_t));

    currentBufferOffset += sizeof(char) + sizeof(size_t);
    packet.body.payload = (char *)malloc(sizeof(char) * packet.body.payloadSize);
    memcpy(packet.body.payload, buffer + currentBufferOffset, sizeof(char) * packet.body.payloadSize);

    size_t parityBufferSize = size - bodyOffset;

    size_t parityBuffSz = 0;
    char *parityBuffer = NULL;
    calculateParity(buffer + bodyOffset, parityBufferSize, &parityBuffer, &parityBuffSz);

    int comp = memcmp(packet.header.parity, parityBuffer, sizeof(char) * packet.header.paritySize);

    free(packet.header.parity);
    free(parityBuffer);

    *body_out = packet.body;
    *validationResult = comp == 0 ? 1 : 0;
}

void radioSendPacket(lora_data_t *lora, radio_body_t *body)
{
    char *buffer;
    size_t bufferSize = 0;
    serializeRadioPacket(body, &buffer, &bufferSize);

    encryptDecrypt(buffer, bufferSize, RADIO_PACKET_KEY, sizeof(RADIO_PACKET_KEY) / sizeof(char));

    loraBeginPacket(lora, 0);
    loraWrite_str_s(lora, buffer, bufferSize);
    loraEndPacket(lora, 0);

    free(buffer);
}

int radioReceivePacket(lora_data_t *lora, radio_body_t *body)
{
    size_t packetSize = loraParsePacket(lora, 0);

    if (packetSize)
    {
        char buffer[packetSize];
        size_t i = 0;

        while (loraAvailable(lora))
        {
            buffer[i] = (char)loraRead(lora);
            i++;
        }

        encryptDecrypt(buffer, packetSize, RADIO_PACKET_KEY, sizeof(RADIO_PACKET_KEY) / sizeof(char));

        int validationResult = 0;
        deserializeRadioPacket(buffer, packetSize, body, &validationResult);

        return 1;
    }

    return 0;
}