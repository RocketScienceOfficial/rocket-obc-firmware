#include "test_time_tracker.h"
#include "time_tracker.h"

MY_TEST_INIT_FUNC(TIME_TRACKER_TEST_NAME)
{
    MY_ASSERT(getMsSinceBoot() >= 5000);

    MY_TEST_END();
}