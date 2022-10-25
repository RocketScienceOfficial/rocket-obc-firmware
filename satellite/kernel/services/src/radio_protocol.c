#include "kernel/services/radio_protocol.h"
#include "tools/cryptography.h"
#include <string.h>
#include <stdlib.h>

static const BYTE RADIO_PACKET_KEY[] = {
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

VOID serializeRadioPacket(RadioBody *body, BYTE **buffer_out_ptr, SIZE *size_out)
{
    SIZE bodySize = sizeof(BYTE) + sizeof(BYTE[3]) + sizeof(SIZE) + sizeof(BYTE) * body->payloadSize;
    BYTE bodyBuffer[bodySize];

    memcpy(bodyBuffer, &body->command, sizeof(BYTE) + sizeof(BYTE[3]) + sizeof(SIZE));
    memcpy(bodyBuffer + sizeof(BYTE) + sizeof(BYTE[3]) + sizeof(SIZE), body->payload, sizeof(BYTE) * body->payloadSize);

    ParityData parity = {0};

    if (!calculateParity(bodyBuffer, bodySize, &parity))
    {
        return;
    }

    _RadioHeader header = {
        ._paritySize = parity.size,
        ._parity = parity.buffer,
    };

    SIZE headerSize = sizeof(SIZE) + sizeof(BYTE) * header._paritySize;
    BYTE headerBuffer[headerSize];

    memcpy(headerBuffer, &header._paritySize, sizeof(SIZE));
    memcpy(headerBuffer + sizeof(SIZE), header._parity, sizeof(BYTE) * header._paritySize);

    if (!clearParity(&parity))
    {
        return;
    }

    SIZE bufferSize = headerSize + bodySize;
    *buffer_out_ptr = (BYTE *)malloc(bufferSize);
    *size_out = bufferSize;

    memcpy(*buffer_out_ptr, headerBuffer, headerSize);
    memcpy(*buffer_out_ptr + headerSize, bodyBuffer, bodySize);

    if (!encryptDecrypt(*buffer_out_ptr, bufferSize, RADIO_PACKET_KEY, sizeof(RADIO_PACKET_KEY) / sizeof(BYTE)))
    {
        return;
    }

    return;
}

VOID deserializeRadioPacket(BYTE *buffer, SIZE size, RadioBody *body_out, BOOL *validationResult)
{
    _RadioPacket packet = {0};

    if (!encryptDecrypt(buffer, size, RADIO_PACKET_KEY, sizeof(RADIO_PACKET_KEY) / sizeof(BYTE)))
    {
        return;
    }

    SIZE currentBufferOffset = 0;
    memcpy(&packet._header, buffer + currentBufferOffset, sizeof(SIZE));

    if (packet._header._paritySize > 0 && packet._header._paritySize <= size)
    {
        currentBufferOffset += sizeof(SIZE);
        packet._header._parity = (BYTE *)malloc(sizeof(BYTE) * packet._header._paritySize);
        memcpy(packet._header._parity, buffer + currentBufferOffset, packet._header._paritySize * sizeof(BYTE));

        SIZE bodyOffset = currentBufferOffset + packet._header._paritySize * sizeof(BYTE);
        SIZE parityBufferSize = size - bodyOffset;

        ParityData parity = {0};

        if (!calculateParity(buffer + bodyOffset, parityBufferSize, &parity))
        {
            return;
        }

        int comp = memcmp(packet._header._parity, parity.buffer, sizeof(BYTE) * packet._header._paritySize);
        *validationResult = comp == 0;

        if (*validationResult)
        {
            currentBufferOffset += packet._header._paritySize * sizeof(BYTE);
            memcpy(&packet._body, buffer + currentBufferOffset, sizeof(BYTE) + sizeof(BYTE[3]) + sizeof(SIZE));

            currentBufferOffset += sizeof(BYTE) + sizeof(BYTE[3]) + sizeof(SIZE);
            packet._body.payload = (BYTE *)malloc(sizeof(BYTE) * packet._body.payloadSize);
            memcpy(packet._body.payload, buffer + currentBufferOffset, sizeof(BYTE) * packet._body.payloadSize);

            *body_out = packet._body;
        }

        free(packet._header._parity);

        if (!clearParity(&parity))
        {
            return;
        }
    }
    else
    {
        *validationResult = false;
    }

    return;
}

VOID radioClearPacket(RadioBody *body)
{
    if (!body)
    {
        return;
    }

    if (body->payload)
    {
        free(body->payload);
    }
}