#include "test_barometer.h"
#include "barometer.h"
#include "pinout.h"
#include "time_tracker.h"

static unsigned int s_TimerOffset = 0;

MY_TEST_INIT_FUNC(BAROMETER_TEST_NAME)
{
    MY_ASSERT(barometerInit(BMP280_I2C, BMP280_I2C_SDA_PIN, BMP280_I2C_SCL_PIN));

    MY_TEST_END();
}

MY_TEST_FUNC(BAROMETER_TEST_NAME, 1)
{
    MY_ASSERT(barometerCheck());

    barometer_data_t barometerData;
    barometerRead(&barometerData);

    MY_ASSERT(barometerData.pressure > 50000);
    MY_ASSERT(barometerData.pressure < 200000);
    MY_ASSERT(barometerData.temperature > 5);
    MY_ASSERT(barometerData.temperature < 50);

    MY_TEST_END();
}

MY_TEST_FUNC_DYNAMIC(BAROMETER_TEST_NAME, 1)
{
    if (runEvery(1000, &s_TimerOffset))
    {
        MY_ASSERT(barometerCheck());

        barometer_data_t barometerData;
        barometerRead(&barometerData);

        MY_ASSERT(barometerData.pressure > 50000);
        MY_ASSERT(barometerData.pressure < 200000);
        MY_ASSERT(barometerData.temperature > 5);
        MY_ASSERT(barometerData.temperature < 50);
    }

    MY_TEST_END();
}