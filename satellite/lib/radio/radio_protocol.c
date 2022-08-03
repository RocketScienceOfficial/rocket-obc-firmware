#include "radio_protocol.h"
#include "cryptography.h"
#include "logger.h"
#include "recorder.h"
#include <string.h>
#include <stdlib.h>

const uint8_t RADIO_PACKET_KEY[] = {
    0x18,
    0x07,
    0x45,
    0x95,
    0xc0,
    0x24,
    0xb0,
    0x49,
    0x06,
    0xfe,
    0x95,
    0x4c,
    0xe7,
    0x10,
    0xfd,
    0x3c,
    0x5f,
    0x4e,
    0xc1,
    0x2f,
    0xed,
    0xdb,
    0xb3,
    0xd1,
    0x82,
    0x45,
    0x9d,
    0x74,
    0x31,
    0x0b,
    0xd6,
    0x82,
    0x3d,
    0xb1,
    0x97,
    0xdd,
    0x01,
    0x30,
    0x7e,
    0x87,
    0xee,
    0x9e,
    0x15,
    0xdf,
    0x1f,
    0x6d,
    0x8c,
    0x16,
    0x99,
    0x37,
    0xb0,
    0xc0,
    0x9f,
    0x56,
    0xf8,
    0x5d,
    0xbd,
    0x5e,
    0xd4,
    0xae,
    0x4d,
    0x23,
    0x6c,
    0xa0,
};

void serializeRadioPacket(radio_body_t *body, uint8_t **buffer_out_ptr, size_t *size_out)
{
    FUNCTION_PROFILE_BEGIN();

    MY_LOG_CORE_INFO("Serializing radio packet...");

    size_t bodySize = sizeof(uint8_t) + sizeof(uint8_t[3]) + sizeof(size_t) + sizeof(uint8_t) * body->payloadSize;
    uint8_t bodyBuffer[bodySize];

    memcpy(bodyBuffer, &body->command, sizeof(uint8_t) + sizeof(uint8_t[3]) + sizeof(size_t));
    memcpy(bodyBuffer + sizeof(uint8_t) + sizeof(uint8_t[3]) + sizeof(size_t), body->payload, sizeof(uint8_t) * body->payloadSize);

    parity_data_t parity = {0};

    calculateParity(bodyBuffer, bodySize, &parity);

    radio_header_t header = {
        .paritySize = parity.size,
        .parity = parity.buffer,
    };

    size_t headerSize = sizeof(size_t) + sizeof(uint8_t) * header.paritySize;
    uint8_t headerBuffer[headerSize];

    memcpy(headerBuffer, &header.paritySize, sizeof(size_t));
    memcpy(headerBuffer + sizeof(size_t), header.parity, sizeof(uint8_t) * header.paritySize);

    clearParity(&parity);

    size_t bufferSize = headerSize + bodySize;
    *buffer_out_ptr = (uint8_t *)malloc(bufferSize);
    *size_out = bufferSize;

    memcpy(*buffer_out_ptr, headerBuffer, headerSize);
    memcpy(*buffer_out_ptr + headerSize, bodyBuffer, bodySize);

    MY_LOG_CORE_INFO("Successfully serialized radio packet!");

    FUNCTION_PROFILE_END();
}

void deserializeRadioPacket(uint8_t *buffer, size_t size, radio_body_t *body_out, int *validationResult)
{
    FUNCTION_PROFILE_BEGIN();

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
        packet.header.parity = (uint8_t *)malloc(sizeof(uint8_t) * packet.header.paritySize);
        memcpy(packet.header.parity, buffer + currentBufferOffset, packet.header.paritySize * sizeof(uint8_t));

        size_t bodyOffset = currentBufferOffset + packet.header.paritySize * sizeof(uint8_t);
        size_t parityBufferSize = size - bodyOffset;

        parity_data_t parity = {0};
        calculateParity(buffer + bodyOffset, parityBufferSize, &parity);

        int comp = memcmp(packet.header.parity, parity.buffer, sizeof(uint8_t) * packet.header.paritySize);
        *validationResult = comp == 0 ? 1 : 0;

        if (*validationResult)
        {
            currentBufferOffset += packet.header.paritySize * sizeof(uint8_t);
            memcpy(&packet.body, buffer + currentBufferOffset, sizeof(uint8_t) + sizeof(uint8_t[3]) + sizeof(size_t));

            currentBufferOffset += sizeof(uint8_t) + sizeof(uint8_t[3]) + sizeof(size_t);
            packet.body.payload = (uint8_t *)malloc(sizeof(uint8_t) * packet.body.payloadSize);
            memcpy(packet.body.payload, buffer + currentBufferOffset, sizeof(uint8_t) * packet.body.payloadSize);

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

    FUNCTION_PROFILE_END();
}

void radioSendPacket(lora_data_t *lora, radio_body_t *body)
{
    FUNCTION_PROFILE_BEGIN();

    MY_LOG_CORE_INFO("Sending packet...");

    uint8_t *buffer;
    size_t bufferSize = 0;
    serializeRadioPacket(body, &buffer, &bufferSize);

    encryptDecrypt(buffer, bufferSize, RADIO_PACKET_KEY, sizeof(RADIO_PACKET_KEY) / sizeof(uint8_t));

    loraBeginPacket(lora, 0);
    loraWrite_str_s(lora, buffer, bufferSize);
    loraEndPacket(lora, 0);

    free(buffer);

    MY_LOG_CORE_INFO("Successfully sent packet!");

    FUNCTION_PROFILE_END();
}

int radioCheckPacket(lora_data_t *lora)
{
    return loraParsePacket(lora, 0);
}

int radioReceivePacket(lora_data_t *lora, radio_body_t *body_out_ptr, int *validationResult_out_ptr)
{
    FUNCTION_PROFILE_BEGIN();

    size_t packetSize = loraParsePacket(lora, 0);

    if (packetSize)
    {
        MY_LOG_CORE_INFO("Receiving packet...");

        uint8_t buffer[packetSize];
        size_t i = 0;

        while (loraAvailable(lora))
        {
            buffer[i] = (uint8_t)loraRead(lora);
            i++;
        }

        encryptDecrypt(buffer, packetSize, RADIO_PACKET_KEY, sizeof(RADIO_PACKET_KEY) / sizeof(uint8_t));

        deserializeRadioPacket(buffer, packetSize, body_out_ptr, validationResult_out_ptr);

        MY_LOG_CORE_INFO("Validation result: %d", *validationResult_out_ptr);
        MY_LOG_CORE_INFO("Successfully received packet!");

        FUNCTION_PROFILE_END();

        return 1;
    }

    FUNCTION_PROFILE_END();

    return 0;
}

void radioClearPacket(radio_body_t *body)
{
    FUNCTION_PROFILE_BEGIN();

    MY_LOG_CORE_INFO("Clearing packet...");

    if (body->payload)
    {
        free(body->payload);
    }

    FUNCTION_PROFILE_END();
}