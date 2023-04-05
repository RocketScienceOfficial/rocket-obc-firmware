#include "drivers/lora/sx127X_driver.h"
#include "pico/stdlib.h"

#define SPI 0
#define SCK 18
#define MOSI 19
#define MISO 16
#define CS 17
#define RESET 22

int main()
{
    stdio_init_all();
    sleep_ms(5000);

    spiInitAll(SPI, 12500);

    SX127XData data = {0};
    SX127XPinout pinout = {
        .cs = CS,
        .spi = SPI,
        .sck = SCK,
        .miso = MISO,
        .mosi = MOSI,
        .reset = RESET,
    };
    sx127XInit(&data, &pinout, 433E6);

    while (TRUE)
    {
        sleep_ms(1000);
    }

    return 0;
}