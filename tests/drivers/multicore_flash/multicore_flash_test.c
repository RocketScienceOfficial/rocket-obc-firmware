#include "tests/test_framework.h"
#include "modules/drivers/hal/multicore.h"
#include "modules/drivers/storage/flash_driver.h"
#include <stdbool.h>

static void core_func()
{
    hal_core_set_as_victim();
    hal_core_init_fifo(1, sizeof(int), 1);

    int data;
    hal_core_receive_data(0, &data);

    TEST_ASSERT(data == 10);

    int newData = 20;
    hal_core_send_data(1, &newData);
}

int main()
{
    TEST_START();

    TEST_START_SECTION("Save on other core");
    {
        hal_core_init_fifo(0, sizeof(int), 1);
        hal_core_start_next(core_func);

        SAFE_FLASH_EXECUTE_MULTICORE(flash_erase_sectors(0, 1));

        int data = 10;
        hal_core_send_data(0, &data);

        int newData;
        hal_core_receive_data(1, &newData);

        TEST_ASSERT(newData == 20);
    }
    TEST_END_SECTION();

    TEST_END();
}