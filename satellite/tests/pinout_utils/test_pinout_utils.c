#include "test_pinout_utils.h"
#include "pinout_utils.h"

MY_TEST_FUNC(PINOUT_UTILS_TEST_NAME, 1)
{
    MY_ASSERT(pinoutCheckPin(0));
    MY_ASSERT(pinoutCheckPin(29) == false);

    MY_TEST_END();
}

MY_TEST_FUNC(PINOUT_UTILS_TEST_NAME, 2)
{
    MY_ASSERT(pinoutCheckI2C(0));
    MY_ASSERT(pinoutCheckI2C(1));
    MY_ASSERT(pinoutCheckI2C(2) == false);

    MY_TEST_END();
}

MY_TEST_FUNC(PINOUT_UTILS_TEST_NAME, 3)
{
    MY_ASSERT(pinoutCheckI2C_SDA(0, 8));
    MY_ASSERT(pinoutCheckI2C_SDA(1, 18));
    MY_ASSERT(pinoutCheckI2C_SDA(0, 27) == false);

    MY_TEST_END();
}

MY_TEST_FUNC(PINOUT_UTILS_TEST_NAME, 4)
{
    MY_ASSERT(pinoutCheckI2C_SCL(0, 1));
    MY_ASSERT(pinoutCheckI2C_SCL(1, 15));
    MY_ASSERT(pinoutCheckI2C_SCL(0, 12) == false);

    MY_TEST_END();
}