#include "drivers/barometer/bme688_driver.h"
#include "drivers/tools/board_control.h"
#include "drivers/tools/time_tracker.h"
#include "maths/math_utils.h"
#include <stdio.h>

#define SPI 0
#define SCK 18
#define MOSI 19
#define MISO 16
#define CS 4

int main()
{
    boardInit(5000);

    spiInitAll(SPI, 1 * 1000 * 1000);

    BME688Config bme688Config = {0};

    bme688Init(&bme688Config, SPI, MISO, MOSI, SCK, CS);
    bme688SetConfig(&bme688Config, BME688_SENSOR_OSR_8X, BME688_SENSOR_OSR_8X, BME688_SENSOR_OSR_16X, BME688_IIR_FILTER_COEFF_OFF);
    bme688SetHeaterConfig(&bme688Config, 0, 0, 300, 0, 100);

    while (TRUE)
    {
        bme688SetMode(&bme688Config, BME688_MODE_FORCED);
        sleepMiliseconds(100);

        BME688Data data = {0};
        bme688Read(&bme688Config, &data);

        printf("Temperature: %f\n", data.temperature);
        printf("Pressure: %d\n", data.pressure);
        printf("Humidity: %f\n", data.humidity);
        printf("Gas: %d\n", data.gas);
        printf("Height: %f\n", heightFromBarometricFormula(data.pressure, data.temperature));
    }

    return 0;
}