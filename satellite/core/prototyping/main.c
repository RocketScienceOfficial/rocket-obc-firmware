#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "pico/stdlib.h"
#include "logging/logger.h"
#include "logging/log_serial.h"
#include "myhardware/barometer.h"
#include "myhardware/flash.h"
#include "utils/time_tracker.h"
#include "shared/hardware_utils.h"

#define DEBUG_MODE 1
#define MEASURE_DELAY_MS 50
#define MEASURE_THRESHOLD_METERS (DEBUG_MODE ? 0.3f : 10)
#define MEASURE_APOGEE_DIFF_THRESHOLD_COUNT 20
#define MEASURE_APOGEE_DIFF_THRESHOLD_METERS 0.2f
#define FLASH_FILE_INDEX 0
#define BMP280_I2C 1
#define BMP280_I2C_SDA_PIN 10
#define BMP280_I2C_SCL_PIN 11
#define PARACHUTE_PIN PICO_DEFAULT_LED_PIN
#define PARACHUTE_HIGH_TIME_MS 1000

static BarometerConfig s_BarometerConfig;
static Timer s_Timer;
static float s_ApogeeAlt;
static int s_ApogeeDiffCount;
static bool s_ApogeeReached;

float getAlt()
{
    BarometerData barometerData = {0};
    float altitude = 0;

    HW_CALL(barometerRead(&s_BarometerConfig, &barometerData));
    HW_CALL(barometerGetAltitude(&s_BarometerConfig, &barometerData, &altitude));

    return altitude;
}

void readApogeeFromFlash()
{
    const uint8_t *buffer;
    size_t buffSize = 0;
    HW_CALL(flashGetFile(getDefaultFlashModule(), FLASH_FILE_INDEX, &buffer, &buffSize));

    if (buffSize)
    {
        float apogee;
        memcpy(&apogee, buffer, sizeof(apogee));

        MY_LOG_CORE_INFO("LAST APOGEE: %f", apogee);
    }
    else
    {
        MY_LOG_CORE_INFO("NO APOGEE IN FLASH");
    }

    HW_CALL(flashClearFile(getDefaultFlashModule(), FLASH_FILE_INDEX));
}

void saveApogeeToFlash()
{
    size_t bufferSize = sizeof(s_ApogeeAlt);
    uint8_t buffer[bufferSize];
    memcpy(buffer, &s_ApogeeAlt, sizeof(s_ApogeeAlt));

    HW_CALL(flashWriteFileBuff(getDefaultFlashModule(), FLASH_FILE_INDEX, buffer, bufferSize));
    HW_CALL(flashTerminate(getDefaultFlashModule()));

    MY_LOG_CORE_INFO("Apogee saved to flash");
}

int main()
{
    stdio_init_all();
    sleep_ms(8000);

    myLogCreateConsoleSink(myLogGetCoreLogger(), DEFAULT_LOG_SERIAL_PATTERN);

    HW_CALL(barometerInit(&s_BarometerConfig, BMP280_I2C, BMP280_I2C_SDA_PIN, BMP280_I2C_SCL_PIN));

    sleep_ms(2000);

    float baseAlt = getAlt();

    MY_LOG_CORE_INFO("Base altitude: %f", baseAlt);

    gpio_init(PARACHUTE_PIN);
    gpio_set_dir(PARACHUTE_PIN, GPIO_OUT);

    readApogeeFromFlash();

    MY_LOG_CORE_INFO("Initialized!");

    while (true)
    {
        if (runEvery(MEASURE_DELAY_MS, &s_Timer))
        {
            if (!s_ApogeeReached)
            {
                float diff = getAlt() - baseAlt;

                if (DEBUG_MODE)
                {
                    MY_LOG_CORE_INFO("Diff: %f", diff);
                }

                if (diff > MEASURE_THRESHOLD_METERS)
                {
                    if (s_ApogeeAlt < diff)
                    {
                        s_ApogeeAlt = diff;
                        s_ApogeeDiffCount = 0;
                    }

                    if (s_ApogeeAlt - diff >= MEASURE_APOGEE_DIFF_THRESHOLD_METERS)
                    {
                        s_ApogeeDiffCount++;

                        if (s_ApogeeDiffCount >= MEASURE_APOGEE_DIFF_THRESHOLD_COUNT)
                        {
                            s_ApogeeDiffCount = 0;
                            s_ApogeeReached = true;

                            MY_LOG_CORE_INFO("Apogee reached! Altitude: %f", s_ApogeeAlt);

                            saveApogeeToFlash();

                            gpio_put(PARACHUTE_PIN, 1);
                            sleep_ms(PARACHUTE_HIGH_TIME_MS);
                            gpio_put(PARACHUTE_PIN, 0);
                        }
                    }
                    else
                    {
                        s_ApogeeDiffCount = 0;
                    }
                }
            }
        }
    }

    return 0;
}