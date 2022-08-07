#include "test_barometer.h"
#include "barometer.h"
#include "pinout.h"
#include "time_tracker.h"

static Timer s_TimerOffset = 0;
static BarometerConfig s_Config;

MY_TEST_INIT_FUNC(BAROMETER_TEST_NAME)
{
    MY_ASSERT(FUNCSUCCESS(barometerInit(&s_Config, BMP280_I2C, BMP280_I2C_SDA_PIN, BMP280_I2C_SCL_PIN)));

    MY_TEST_END();
}

MY_TEST_FUNC(BAROMETER_TEST_NAME, 1)
{
    bool checkResult;
    MY_ASSERT(FUNCSUCCESS(barometerCheck(&s_Config, &checkResult)));
    MY_ASSERT(checkResult);

    BarometerData barometerData;
    MY_ASSERT(FUNCSUCCESS(barometerRead(&s_Config, &barometerData)));

    MY_ASSERT(barometerData.pressure > 50000);
    MY_ASSERT(barometerData.pressure < 200000);
    MY_ASSERT(barometerData.temperature > 5);
    MY_ASSERT(barometerData.temperature < 50);

    MY_TEST_END();
}

MY_TEST_FUNC(BAROMETER_TEST_NAME, 2)
{
    BarometerConfig config;

    MY_ASSERT(FUNCFAILED(barometerInit(NULL, BMP280_I2C, BMP280_I2C_SDA_PIN, BMP280_I2C_SCL_PIN)));
    MY_ASSERT(FUNCFAILED(barometerInit(&config, 0, 2, 3)));

    MY_TEST_END();
}

MY_TEST_FUNC(BAROMETER_TEST_NAME, 3)
{
    MY_ASSERT(FUNCFAILED(barometerCheck(NULL, NULL)));

    MY_TEST_END();
}

MY_TEST_FUNC(BAROMETER_TEST_NAME, 4)
{
    MY_ASSERT(FUNCFAILED(barometerRead(NULL, NULL)));

    MY_TEST_END();
}

MY_TEST_FUNC_DYNAMIC(BAROMETER_TEST_NAME, 1)
{
    if (runEvery(1000, &s_TimerOffset))
    {
        bool checkResult;
        MY_ASSERT(FUNCSUCCESS(barometerCheck(&s_Config, &checkResult)));
        MY_ASSERT(checkResult);

        BarometerData barometerData;
        MY_ASSERT(FUNCSUCCESS(barometerRead(&s_Config, &barometerData)));

        MY_ASSERT(barometerData.pressure > 50000);
        MY_ASSERT(barometerData.pressure < 200000);
        MY_ASSERT(barometerData.temperature > 5);
        MY_ASSERT(barometerData.temperature < 50);

        MY_TEST_END();
    }

    MY_TEST_END();
}