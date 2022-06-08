#include "pico/stdlib.h"
#include "pinout.h"
#include "bmp280.h"
#include "test_framework.h"
#include "logger.h"
#include "log_serial.h"
#include "time_tracker.h"

static unsigned int s_TimerOffset = 0;

void start();
void initialize();
void loop();

int main()
{
    start();

    initialize();

    while (1)
    {
        loop();
    }

    return 0;
}

void start()
{
    stdio_init_all();
    sleep_ms(5000);
}

void initialize()
{
    attachSerialToLog(myLogGetCoreLogger());

    MY_LOG_CORE_INFO("Initializing...");

    bmp280Init(BMP280_I2C, BMP280_I2C_SDA_PIN, BMP280_I2C_SCL_PIN);

    MY_LOG_CORE_INFO("Everything is ready!");
}

void loop()
{
    if (runEvery(1000, &s_TimerOffset))
    {
        bmp280_data_t bmp280Data;
        bmp280Read(&bmp280Data);

        MY_LOG_CORE_INFO("BMP280 Pressure: %f Pa", bmp280Data.pressure);
        MY_LOG_CORE_INFO("BMP280 Temperature: %f C", bmp280Data.temperature);
    }
}