#include "tests/test_framework.h"
#include "lib/crypto/crc.h"

int main()
{
    TEST_START();

    TEST_START_SECTION("CRC-16/CCITT");
    {
        const uint8_t *data = "123456789";
        uint16_t crc = crc16_ccitt_calculate(data, 9);

        TEST_ASSERT(crc == 0x29B1);
    }
    TEST_END_SECTION();

    TEST_START_SECTION("CRC-16/MCRF4XX");
    {
        const uint8_t *data = "123456789";
        uint16_t crc = crc16_mcrf4xx_calculate(data, 9);

        TEST_ASSERT(crc == 0x6F91);
    }
    TEST_END_SECTION();

    TEST_END();
}