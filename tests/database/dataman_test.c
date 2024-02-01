#include "tests/test_framework.h"
#include "modules/database/dataman.h"
#include "modules/database/dataman_frames.h"
#include "modules/database/db_flash_sectors.h"
#include "modules/maths/vector.h"

int main()
{
    TEST_START();

    TEST_START_SECTION("Dataman clear");
    {
        dataman_clear();
    }
    TEST_END_SECTION();

    TEST_START_SECTION("Dataman write");
    {
        dataman_sensor_frame_t frame = {
            .acc1 = (vec3_t){
                .x = 10,
                .y = 20,
                .z = 30,
            },
        };
        dataman_save_sensors_frame(&frame);
        dataman_save_sensors_frame(&frame);
        dataman_save_sensors_frame(&frame);
        dataman_save_sensors_frame(&frame);
        dataman_save_sensors_frame(&frame);
        dataman_save_sensors_frame(&frame);
        dataman_save_sensors_frame(&frame);
        dataman_save_sensors_frame(&frame);
        dataman_save_sensors_frame(&frame);
        dataman_save_sensors_frame(&frame);
        dataman_flush();
    }
    TEST_END_SECTION();

    TEST_START_SECTION("Dataman read");
    {
        TEST_ASSERT(dataman_read() == 10);
    }
    TEST_END_SECTION();

    TEST_END();
}