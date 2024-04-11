#include "tests/test_framework.h"
#include "modules/radiolink/radiolink.h"
#include "modules/radiolink/radiolink_messages.h"
#include "modules/maths/vector.h"

int main()
{
    TEST_START();

    TEST_START_SECTION("Serialize sensor frame");
    {
        radiolink_sensor_frame_t sensorFrame = {0};
        sensorFrame.gyro = (vec3_t){
            .x = 10,
            .y = 20,
            .z = 30,
        };

        radiolink_frame_t frame;
        uint8_t seq = 0;
        radiolink_serialize_sensor_frame(&frame, &seq, &sensorFrame);

        TEST_ASSERT(frame.len == sizeof(radiolink_sensor_frame_t));
        TEST_ASSERT(frame.magic == RADIOLINK_MAGIC);
        TEST_ASSERT(frame.seq == 0);
        TEST_ASSERT(seq == 1);

        uint8_t buffer[512];
        size_t len = sizeof(buffer);
        radiolink_get_bytes(&frame, buffer, &len);

        TEST_ASSERT(len > 0);

        radiolink_frame_t rawFrame;
        bool deserializeResult = radiolink_deserialize(&rawFrame, buffer, len);

        TEST_ASSERT(deserializeResult);
        TEST_ASSERT(rawFrame.msgId == RADIOLINK_MESSAGE_SENSORS);

        radiolink_sensor_frame_t *rawSensorFrame = (radiolink_sensor_frame_t *)rawFrame.payload;

        TEST_ASSERT(rawSensorFrame->gyro.x == 10);
        TEST_ASSERT(rawSensorFrame->gyro.y == 20);
        TEST_ASSERT(rawSensorFrame->gyro.z == 30);
    }
    TEST_END_SECTION();

    TEST_START_SECTION("Get bytes buffer too small");
    {
        radiolink_sensor_frame_t sensorFrame = {0};
        radiolink_frame_t frame;
        uint8_t seq = 0;
        radiolink_serialize_sensor_frame(&frame, &seq, &sensorFrame);

        uint8_t buffer[10];
        size_t len = sizeof(buffer);
        bool result = radiolink_get_bytes(&frame, buffer, &len);

        TEST_ASSERT(!result);
    }
    TEST_END_SECTION();

    TEST_START_SECTION("Deserialize random bytes");
    {
        uint8_t buffer[] = {0x27, 0x19, 0x12};
        size_t len = sizeof(buffer);

        radiolink_frame_t frame;
        bool result = radiolink_deserialize(&frame, buffer, len);

        TEST_ASSERT(!result);
    }
    TEST_END_SECTION();

    TEST_START_SECTION("Deserialize random bytes 2");
    {
        uint8_t buffer[] = {0x27, 0x19, 0x12, 0x92, 0xF1, 0xB5, 0x13, 0x46};
        size_t len = sizeof(buffer);

        radiolink_frame_t frame;
        bool result = radiolink_deserialize(&frame, buffer, len);

        TEST_ASSERT(!result);
    }
    TEST_END_SECTION();

    TEST_START_SECTION("Invalid checksum");
    {
        radiolink_sensor_frame_t sensorFrame = {0};
        radiolink_frame_t frame;
        uint8_t seq = 0;
        radiolink_serialize_sensor_frame(&frame, &seq, &sensorFrame);

        uint8_t buffer[512];
        size_t len = sizeof(buffer);
        radiolink_get_bytes(&frame, buffer, &len);

        buffer[len - 1] = 0xFF;

        radiolink_frame_t rawFrame;
        bool deserializeResult = radiolink_deserialize(&rawFrame, buffer, len);

        TEST_ASSERT(!deserializeResult);
    }
    TEST_END_SECTION();

    TEST_START_SECTION("Invalid payload length");
    {
        radiolink_sensor_frame_t sensorFrame = {0};
        radiolink_frame_t frame;
        uint8_t seq = 0;
        radiolink_serialize_sensor_frame(&frame, &seq, &sensorFrame);

        uint8_t buffer[512];
        size_t len = sizeof(buffer);
        radiolink_get_bytes(&frame, buffer, &len);

        buffer[1] = 0xFB;

        radiolink_frame_t rawFrame;
        bool deserializeResult = radiolink_deserialize(&rawFrame, buffer, len);

        TEST_ASSERT(!deserializeResult);
    }
    TEST_END_SECTION();

    TEST_START_SECTION("Test sequence overlap");
    {
        radiolink_sensor_frame_t sensorFrame = {0};
        radiolink_frame_t frame;
        uint8_t seq = 255;
        radiolink_serialize_sensor_frame(&frame, &seq, &sensorFrame);

        TEST_ASSERT(frame.seq == 255);
        TEST_ASSERT(seq == 0);
    }
    TEST_END_SECTION();

    TEST_END();
}