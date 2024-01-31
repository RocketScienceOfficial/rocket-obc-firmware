#include "tests/test_framework.h"
#include "modules/maths/crc.h"
#include "modules/maths/math_utils.h"
#include <math.h>

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

    TEST_START_SECTION("Fast inverse sqrt");
    {
        float val = fast_inv_sqrt(2.0f);
        float expected = 1.0f / sqrtf(2.0f);

        TEST_ASSERT(value_approx_eql(val, expected, 0.00001f));
    }
    TEST_END_SECTION();

    TEST_START_SECTION("Clamp - Max");
    {
        float val = clamp_value(11, 9, 10);

        TEST_ASSERT(val == 10);
    }
    TEST_END_SECTION();

    TEST_START_SECTION("Clamp - Min");
    {
        float val = clamp_value(-9, -8, 10);

        TEST_ASSERT(val == -8);
    }
    TEST_END_SECTION();

    TEST_START_SECTION("Barometric formula");
    {
        float val = height_from_baro_formula(98000, 15);

        TEST_ASSERT(value_approx_eql(val, 280.53f, 0.1f));
    }
    TEST_END_SECTION();

    TEST_END();
}