#include "tests/test_framework.h"
#include "modules/sm/sm_control.h"

int main()
{
    TEST_START();

    TEST_START_SECTION("Test states");
    {
        sm_data_t data;
        sm_input_t inp;

        sm_init(&data);

        inp = (sm_input_t){0, 0, 100, 0};
        sm_update(&data, &inp);
        TEST_ASSERT(data.state == FLIGHT_STATE_ACCELERATING);
    }
    TEST_END_SECTION();

    TEST_END();
}