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
    MY_LOG_CORE_INFO("Serializing radio packet...");

    size_t bodySize = sizeof(char) + sizeof(char[3]) + sizeof(size_t) + sizeof(char) * body->payloadSize;
    char bodyBuffer[bodySize];

    memcpy(bodyBuffer, &body->command, sizeof(char) + sizeof(char[3]) + sizeof(size_t));
    memcpy(bodyBuffer + sizeof(char) + sizeof(char[3]) + sizeof(size_t), body->payload, sizeof(char) * body->payloadSize);

    parity_data_t parity = {0};

    calculateParity(bodyBuffer, bodySize, &parity);

    radio_header_t header = {
        .paritySize = parity.size,
        .parity = parity.buffer,
    };

    size_t headerSize = sizeof(size_t) + sizeof(char) * header.paritySize;
    char headerBuffer[headerSize];

    memcpy(headerBuffer, &header.paritySize, sizeof(size_t));
    memcpy(headerBuffer + sizeof(size_t), header.parity, sizeof(char) * header.paritySize);

    clearParity(&parity);

    size_t bufferSize = headerSize + bodySize;
    *buffer_out_ptr = (char *)malloc(bufferSize);
    *size_out = bufferSize;

    memcpy(*buffer_out_ptr, headerBuffer, headerSize);
    memcpy(*buffer_out_ptr + headerSize, bodyBuffer, bodySize);

    MY_LOG_CORE_INFO("Successfully serialized radio packet!");
}

void deserializeRadioPacket(char *buffer, size_t size, radio_body_t *body_out, int *validationResult)
{
    MY_LOG_CORE_INFO("Deserializing radio packet...");

    radio_packet_t packet = {
        .header = {0},
        .body = {0},
    };

    size_t currentBufferOffset = 0;
    memcpy(&packet.header, buffer + currentBufferOffset, sizeof(size_t));

    MY_LOG_CORE_INFO("Parity Size: %d", packet.header.paritySize);

    if (packet.header.paritySize > 0 && packet.header.paritySize <= size)
    {
        currentBufferOffset += sizeof(size_t);
        packet.header.parity = (char *)malloc(sizeof(char) * packet.header.paritySize);
        memcpy(packet.header.parity, buffer + currentBufferOffset, packet.header.paritySize * sizeof(char));

        size_t bodyOffset = currentBufferOffset + packet.header.paritySize * sizeof(char);
        size_t parityBufferSize = size - bodyOffset;

        parity_data_t parity = {0};
        calculateParity(buffer + bodyOffset, parityBufferSize, &parity);

        int comp = memcmp(packet.header.parity, parity.buffer, sizeof(char) * packet.header.paritySize);
        *validationResult = comp == 0 ? 1 : 0;

        if (*validationResult)
        {
            currentBufferOffset += packet.header.paritySize * sizeof(char);
            memcpy(&packet.body, buffer + currentBufferOffset, sizeof(char) + sizeof(char[3]) + sizeof(size_t));

            currentBufferOffset += sizeof(char) + sizeof(char[3]) + sizeof(size_t);
            packet.body.payload = (char *)malloc(sizeof(char) * packet.body.payloadSize);
            memcpy(packet.body.payload, buffer + currentBufferOffset, sizeof(char) * packet.body.payloadSize);

            *body_out = packet.body;

            MY_LOG_CORE_INFO("Successfully validated radio packet!");
        }
        else
        {
            MY_LOG_CORE_ERROR("Could not validate packet!");
        }

        free(packet.header.parity);
        
        clearParity(&parity);

        MY_LOG_CORE_INFO("Successfully deserialized radio packet!");
    }
    else
    {
        MY_LOG_CORE_ERROR("Invalid parity size!");

        *validationResult = 0;
    }
}

void radioSendPacket(lora_data_t *lora, radio_body_t *body)
{
    MY_LOG_CORE_INFO("Sending packet...");

    char *buffer;
    size_t bufferSize = 0;
    serializeRadioPacket(body, &buffer, &bufferSize);

    encryptDecrypt(buffer, bufferSize, RADIO_PACKET_KEY, sizeof(RADIO_PACKET_KEY) / sizeof(char));

    loraBeginPacket(lora, 0);
    loraWrite_str_s(lora, buffer, bufferSize);
    loraEndPacket(lora, 0);

    free(buffer);

    MY_LOG_CORE_INFO("Successfully sent packet!");
}

int radioReceivePacket(lora_data_t *lora, radio_body_t *body_out_ptr, int* validationResult_out_ptr)
{
    size_t packetSize = loraParsePacket(lora, 0);

    if (packetSize)
    {
        MY_LOG_CORE_INFO("Receiving packet...");

        char buffer[packetSize];
        size_t i = 0;

        while (loraAvailable(lora))
        {
            buffer[i] = (char)loraRead(lora);
            i++;
        }

        encryptDecrypt(buffer, packetSize, RADIO_PACKET_KEY, sizeof(RADIO_PACKET_KEY) / sizeof(char));

        deserializeRadioPacket(buffer, packetSize, body_out_ptr, validationResult_out_ptr);

        MY_LOG_CORE_INFO("Validation result: %d", *validationResult_out_ptr);
        MY_LOG_CORE_INFO("Successfully received packet!");

        return 1;
    }

    return 0;
}

void radioClearPacket(radio_body_t* body)
{
    MY_LOG_CORE_INFO("Clearing packet...");

    if (body->payload)
    {
        free(body->payload);
    }
}