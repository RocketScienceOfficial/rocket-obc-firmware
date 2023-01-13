#include "pico/stdlib.h"
#include "hardware/spi.h"
#include <stdio.h>

#define SPI spi_default
#define MISO PICO_DEFAULT_SPI_RX_PIN
#define MOSI PICO_DEFAULT_SPI_TX_PIN
#define SCK PICO_DEFAULT_SPI_SCK_PIN
#define CS PICO_DEFAULT_SPI_RX_PIN

static uint8_t readReg(uint8_t addr)
{
    uint8_t data[2];

    addr = addr | 0x80;

    gpio_put(CS, 0);
    spi_write_blocking(SPI, &addr, 1);
    spi_read_blocking(SPI, 0, data, 2);
    gpio_put(CS, 1);

    return data[1];
}

static void writeReg(uint8_t addr, uint8_t data)
{
    addr = addr & 0x7F;

    uint8_t buff[2];
    buff[0] = addr;
    buff[1] = data;

    gpio_put(CS, 0);
    spi_write_blocking(SPI, buff, 2);
    gpio_put(CS, 1);
}

int main()
{
    stdio_init_all();
    sleep_ms(5000);

    spi_init(SPI, 10000000);

    gpio_set_function(MISO, GPIO_FUNC_SPI);
    gpio_set_function(MOSI, GPIO_FUNC_SPI);
    gpio_set_function(SCK, GPIO_FUNC_SPI);

    gpio_init(CS);
    gpio_set_dir(CS, GPIO_OUT);
    gpio_put(CS, 1);

    sleep_ms(1000);

    printf("ID: %d\n", readReg(0x00));
    sleep_ms(1000);
    printf("ID: %d\n", readReg(0x00));
    sleep_ms(1000);

    while (1)
    {
        tight_loop_contents();
    }

    return 0;
}