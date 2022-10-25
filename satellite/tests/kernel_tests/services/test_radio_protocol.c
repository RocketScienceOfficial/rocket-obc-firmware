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

    const BYTE EXPECTED_SERIALIZED_BUFFER[] = {0x06, 0x00, 0x00, 0x00, 0x11, 0x60, 0x04, 0x01, 0x01, 0x01, 0x54, 0x00, 0x00, 0x00, 0x0d, 0x00, 0x00, 0x00, 0x48, 0x65, 0x6c, 0x6c, 0x6f, 0x20, 0x57, 0x6f, 0x72, 0x6c, 0x64, 0x21, 0x00};
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