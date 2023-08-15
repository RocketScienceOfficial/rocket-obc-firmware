#include "drivers/lora/sx127X_driver.h"
#include "drivers/tools/board_control.h"
#include <stdio.h>

#define SPI 1
#define SCK 10
#define MISO 11
#define MOSI 12
#define CS 16
#define RESET 18

int main()
{
    boardInit(5000);

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

    FUNCRESULT code = sx127XInit(&data, &pinout, 433E6);

    printf("Result code of init: %d\n", code);

    while (TRUE)
    {
        tightLoop();
    }

    return 0;
}