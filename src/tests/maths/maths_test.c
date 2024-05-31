#include "tests/test_framework.h"
#include "lib/maths/fast_math.h"
#include "lib/maths/math_utils.h"
#include <math.h>

int main()
{
    TEST_START();

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

    TEST_END();
}