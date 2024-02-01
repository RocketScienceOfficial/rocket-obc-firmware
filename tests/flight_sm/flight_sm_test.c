#include "tests/test_framework.h"
#include "modules/flight_sm/flight_sm_control.h"

int main()
{
    TEST_START();

    TEST_START_SECTION("Test states");
    {
        flight_sm_data_t data;
        flight_sm_input_t inp;

        flight_sm_init(&data);

        inp = (flight_sm_input_t){0, 0, 100, 0};
        flight_sm_update(&data, &inp);
        TEST_ASSERT(data.state == FLIGHT_STATE_ACCELERATING);
    }
    TEST_END_SECTION();

    TEST_END();
}