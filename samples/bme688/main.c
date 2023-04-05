#include "drivers/barometer/bme688_driver.h"
#include "pico/stdlib.h"
#include <stdio.h>

#define SPI 0
#define SCK 18
#define MOSI 19
#define MISO 16
#define CS 4

int main()
{
    stdio_init_all();
    sleep_ms(5000);

    spiInitAll(SPI, 1 * 1000 * 1000);

    BME688Config bme688Config = {0};

    bme688Init(&bme688Config, SPI, MISO, MOSI, SCK, CS);
    bme688SetHumidityOSR(&bme688Config, BME688_SENSOR_OSR_16X);
    bme688SetTemperatureOSR(&bme688Config, BME688_SENSOR_OSR_2X);
    bme688SetPressureOSR(&bme688Config, BME688_SENSOR_OSR_1X);
    bme688SetIIRFilter(&bme688Config, BME688_IIR_FILTER_COEFF_OFF);
    __bme688SetGasSensorHeaterOnTime(&bme688Config, 0, 100);
    __bme688SetTargetHeaterTemp(&bme688Config, 0, 300);
    __bme688SetHeaterProfile(&bme688Config, 0);
    __bme688RunGas(&bme688Config);

    while (TRUE)
    {
        bme688SetMode(&bme688Config, BME688_MODE_FORCED);
        sleep_ms(1000);

        BME688Data data = {0};
        bme688Read(&bme688Config, &data);

        printf("Temperature: %f\n", data.temperature);
        printf("Pressure: %f\n", data.pressure);
        printf("Humidity: %f\n", data.humidity);
        printf("Gas: %f\n", data.gas);

        sleep_ms(100);
    }

    return 0;
}