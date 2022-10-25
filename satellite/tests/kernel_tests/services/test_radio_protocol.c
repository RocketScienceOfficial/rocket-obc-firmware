#include "test_radio_protocol.h"
#include "kernel/services/radio_protocol.h"
#include "tools/typedefs.h"
#include <string.h>
#include <stdlib.h>

MY_TEST_FUNC(RADIO_PROTOCOL_TEST_NAME, 1)
{
    BYTE text[] = "Hello World!";

    RadioBody body = {
        .command = 'T',
        .payloadSize = sizeof(text) / sizeof(BYTE),
        .payload = text,
    };

    const BYTE EXPECTED_SERIALIZED_BUFFER[] = {0x1e, 0x07, 0x45, 0x95, 0xd1, 0x44, 0xb4, 0x48, 0x07, 0xff, 0xc1, 0x4c, 0xe7, 0x10, 0xf0, 0x3c, 0x5f, 0x4e, 0x89, 0x4a, 0x81, 0xb7, 0xdc, 0xf1, 0xd5, 0x2a, 0xef, 0x18, 0x55, 0x2a, 0xd6};
    BYTE *buffer;
    size_t size;

    serializeRadioPacket(&body, &buffer, &size);

    MY_ASSERT(memcmp(buffer, EXPECTED_SERIALIZED_BUFFER, sizeof(EXPECTED_SERIALIZED_BUFFER) / sizeof(BYTE)) == 0);

    RadioBody body2 = {0};
    BOOL validationResult;

    deserializeRadioPacket(buffer, size, &body2, &validationResult);

    MY_ASSERT(validationResult);
    MY_ASSERT(body.command == body2.command);
    MY_ASSERT(body.payloadSize == body2.payloadSize);
    MY_ASSERT(memcmp(body.payload, body2.payload, body.payloadSize) == 0);

    free(body2.payload);
    free(buffer);

    MY_TEST_END();
}