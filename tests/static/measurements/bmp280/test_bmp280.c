#include "test_bmp280.h"
#include "bmp280.h"
#include "pinout.h"

MY_TEST_INIT_FUNC(BMP280_TEST_NAME)
{
    MY_ASSERT(bmp280Init(BMP280_I2C, BMP280_I2C_SDA_PIN, BMP280_I2C_SCL_PIN));

    MY_TEST_END();
}

MY_TEST_FUNC(BMP280_TEST_NAME, 1)
{
    MY_ASSERT(bmp280Check());

    bmp280_data_t bmp280Data;
    bmp280Read(&bmp280Data);

    MY_ASSERT(bmp280Data.pressure > 50000);
    MY_ASSERT(bmp280Data.pressure < 200000);
    MY_ASSERT(bmp280Data.temperature > 5);
    MY_ASSERT(bmp280Data.temperature < 50);

    MY_TEST_END();
}