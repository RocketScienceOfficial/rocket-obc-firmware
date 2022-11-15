#include "test_radio_protocol.h"
#include "kernel/services/radio_protocol.h"
#include "tools/typedefs.h"
#include <string.h>
#include <stdlib.h>

static const BYTE EXPECTED_SERIALIZED_BUFFER[] = {
    0x1e,
    0x07,
    0x45,
    0x95,
    0xd1,
    0x44,
    0xb4,
    0x48,
    0x07,
    0xff,
    0xa8,
    0xdb,
    0xaa,
    0x1d,
    0xc8,
    0x7f,
    0x41,
    0x61,
    0xf3,
    0x44,
    0xbb,
    0xf5,
    0x84,
    0xe8,
    0xb2,
    0x7c,
    0xc9,
    0x74,
    0x31,
    0x0b,
    0xdb,
    0x82,
    0x3d,
    0xb1,
    0xdf,
    0xb8,
    0x6d,
    0x5c,
    0x11,
    0xa7,
    0xb9,
    0xf1,
    0x67,
    0xb3,
    0x7b,
    0x4c,
    0x8c,
};

MY_TEST_FUNC(RADIO_PROTOCOL_TEST_NAME, 1)
{
    BYTE text[] = "Hello World!";

    RadioBody body = {
        .command = 'T',
        .payloadSize = sizeof(text) / sizeof(BYTE),
        .payload = text,
    };

    BYTE *buffer;
    size_t size;

    BOOL serializationResult = serializeRadioPacket(&body, &buffer, &size);

    MY_ASSERT(serializationResult);
    MY_ASSERT(memcmp(buffer, EXPECTED_SERIALIZED_BUFFER, sizeof(EXPECTED_SERIALIZED_BUFFER) / sizeof(BYTE)) == 0);

    RadioBody body2 = {0};
    BOOL deserializationResult = deserializeRadioPacket(buffer, size, &body2);

    MY_ASSERT(deserializationResult);
    MY_ASSERT(body.command == body2.command);
    MY_ASSERT(body.payloadSize == body2.payloadSize);
    MY_ASSERT(memcmp(body.payload, body2.payload, body.payloadSize) == 0);

    free(buffer);
    free(body2.payload);

    MY_TEST_END();
}

MY_TEST_FUNC(RADIO_PROTOCOL_TEST_NAME, 2)
{
    SIZE totalSize = sizeof(EXPECTED_SERIALIZED_BUFFER) / sizeof(BYTE) + 2 * sizeof(BYTE);
    BYTE *buffer = (BYTE *)malloc(totalSize);

    memcpy(buffer, EXPECTED_SERIALIZED_BUFFER, sizeof(EXPECTED_SERIALIZED_BUFFER) / sizeof(BYTE));

    buffer[0] = 0x75;
    buffer[totalSize - 2] = 0xE5;
    buffer[totalSize - 1] = 0x9B;

    RadioBody body = {0};
    BOOL deserializationResult = deserializeRadioPacket(buffer, totalSize, &body);

    MY_ASSERT(!deserializationResult);

    free(buffer);

    MY_TEST_END();
}