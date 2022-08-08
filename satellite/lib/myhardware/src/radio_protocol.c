#include "myhardware/radio_protocol.h"
#include "utils/cryptography.h"
#include <string.h>
#include <stdlib.h>

static const uint8_t RADIO_PACKET_KEY[] = {
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

FUNCRESULT serializeRadioPacket(RadioBody *body, uint8_t **buffer_out_ptr, size_t *size_out)
{
    size_t bodySize = sizeof(uint8_t) + sizeof(uint8_t[3]) + sizeof(size_t) + sizeof(uint8_t) * body->payloadSize;
    uint8_t bodyBuffer[bodySize];

    memcpy(bodyBuffer, &body->command, sizeof(uint8_t) + sizeof(uint8_t[3]) + sizeof(size_t));
    memcpy(bodyBuffer + sizeof(uint8_t) + sizeof(uint8_t[3]) + sizeof(size_t), body->payload, sizeof(uint8_t) * body->payloadSize);

    ParityData parity = {0};

    if (!calculateParity(bodyBuffer, bodySize, &parity))
    {
        return ERR_FAIL;
    }

    _RadioHeader header = {
        ._paritySize = parity.size,
        ._parity = parity.buffer,
    };

    size_t headerSize = sizeof(size_t) + sizeof(uint8_t) * header._paritySize;
    uint8_t headerBuffer[headerSize];

    memcpy(headerBuffer, &header._paritySize, sizeof(size_t));
    memcpy(headerBuffer + sizeof(size_t), header._parity, sizeof(uint8_t) * header._paritySize);

    if (!clearParity(&parity))
    {
        return ERR_FAIL;
    }

    size_t bufferSize = headerSize + bodySize;
    *buffer_out_ptr = (uint8_t *)malloc(bufferSize);
    *size_out = bufferSize;

    memcpy(*buffer_out_ptr, headerBuffer, headerSize);
    memcpy(*buffer_out_ptr + headerSize, bodyBuffer, bodySize);

    return SUC_OK;
}

FUNCRESULT deserializeRadioPacket(uint8_t *buffer, size_t size, RadioBody *body_out, bool *validationResult)
{
    _RadioPacket packet = {0};

    size_t currentBufferOffset = 0;
    memcpy(&packet._header, buffer + currentBufferOffset, sizeof(size_t));

    if (packet._header._paritySize > 0 && packet._header._paritySize <= size)
    {
        currentBufferOffset += sizeof(size_t);
        packet._header._parity = (uint8_t *)malloc(sizeof(uint8_t) * packet._header._paritySize);
        memcpy(packet._header._parity, buffer + currentBufferOffset, packet._header._paritySize * sizeof(uint8_t));

        size_t bodyOffset = currentBufferOffset + packet._header._paritySize * sizeof(uint8_t);
        size_t parityBufferSize = size - bodyOffset;

        ParityData parity = {0};

        if (!calculateParity(buffer + bodyOffset, parityBufferSize, &parity))
        {
            return ERR_FAIL;
        }

        int comp = memcmp(packet._header._parity, parity.buffer, sizeof(uint8_t) * packet._header._paritySize);
        *validationResult = comp == 0;

        if (*validationResult)
        {
            currentBufferOffset += packet._header._paritySize * sizeof(uint8_t);
            memcpy(&packet._body, buffer + currentBufferOffset, sizeof(uint8_t) + sizeof(uint8_t[3]) + sizeof(size_t));

            currentBufferOffset += sizeof(uint8_t) + sizeof(uint8_t[3]) + sizeof(size_t);
            packet._body.payload = (uint8_t *)malloc(sizeof(uint8_t) * packet._body.payloadSize);
            memcpy(packet._body.payload, buffer + currentBufferOffset, sizeof(uint8_t) * packet._body.payloadSize);

            *body_out = packet._body;
        }

        free(packet._header._parity);

        if (!clearParity(&parity))
        {
            return ERR_FAIL;
        }
    }
    else
    {
        *validationResult = false;
    }

    return SUC_OK;
}

FUNCRESULT radioSendPacket(LoRaData *lora, RadioBody *body)
{
    if (!lora || !body)
    {
        return ERR_INVALIDARG;
    }

    uint8_t *buffer;
    size_t bufferSize = 0;

    if (FUNCFAILED(serializeRadioPacket(body, &buffer, &bufferSize)))
    {
        return ERR_FAIL;
    }

    if (!encryptDecrypt(buffer, bufferSize, RADIO_PACKET_KEY, sizeof(RADIO_PACKET_KEY) / sizeof(uint8_t)))
    {
        return ERR_FAIL;
    }

    loraBeginPacket(lora, 0);
    loraWrite_str_s(lora, buffer, bufferSize);
    loraEndPacket(lora, 0);

    free(buffer);

    return SUC_OK;
}

FUNCRESULT radioReceivePacket(LoRaData *lora, bool *available, RadioBody *body, bool *validationResult)
{
    if (!lora || !available || !body || !validationResult)
    {
        return ERR_INVALIDARG;
    }

    size_t packetSize = loraParsePacket(lora, 0);

    *available = packetSize > 0;

    if (packetSize)
    {
        uint8_t buffer[packetSize];
        size_t i = 0;

        while (loraAvailable(lora))
        {
            buffer[i] = (uint8_t)loraRead(lora);
            i++;
        }

        if (!encryptDecrypt(buffer, packetSize, RADIO_PACKET_KEY, sizeof(RADIO_PACKET_KEY) / sizeof(uint8_t)))
        {
            return ERR_FAIL;
        }

        if (FUNCFAILED(deserializeRadioPacket(buffer, packetSize, body, validationResult)))
        {
            return ERR_FAIL;
        }
    }

    return SUC_OK;
}

FUNCRESULT radioClearPacket(RadioBody *body)
{
    if (!body)
    {
        return ERR_INVALIDARG;
    }

    if (body->payload)
    {
        free(body->payload);
    }

    return SUC_OK;
}