#include "hal/board_control.h"
#include "hal/serial_driver.h"
#include "hal/uart_driver.h"
#include "hal/spi_driver.h"
#include "lib/drivers/lora/sx127X_driver.h"

#define SPI 0
#define CS 2
#define MISO 4
#define MOSI 3
#define SCK 6
#define RESET 7
#define SPI_FREQUENCY 100000

#define LORA_FREQUENCY 433E6
#define LORA_BANDWIDTH 125E3
#define LORA_SF 8

#define UART 1
#define RX 9
#define TX 8
#define UART_BAUDRATE 115200

int main()
{
    hal_board_init(1000);

    hal_serial_printf("Initialized board!\n");
    hal_serial_printf("Firmware version: 1.0\n");
    hal_serial_printf("Author: Filip Gawlik\n");

    hal_spi_init_all(SPI, MISO, MOSI, SCK, SPI_FREQUENCY);
    hal_uart_init_all(UART, RX, TX, UART_BAUDRATE);

    sx127x_data_t loraData = {0};
    sx127x_pinout_t pinoutData = {
        .spi = SPI,
        .cs = CS,
        .reset = RESET,
    };

    sx127x_init(&loraData, &pinoutData, LORA_FREQUENCY);
    sx127x_set_signal_bandwidth(&loraData, LORA_BANDWIDTH);
    sx127x_set_spreading_factor(&loraData, LORA_SF);

    while (true)
    {
        hal_serial_printf("Awaiting for packet...\n");

        uint8_t size;
        hal_uart_read(UART, &size, 1);

        hal_serial_printf("Received request for %d bytes\n", size);

        uint8_t buffer[256];
        hal_uart_read(UART, buffer, size);

        hal_serial_printf("All bytes received!\n");

        sx127x_write_buffer(&loraData, buffer, size);

        hal_serial_printf("Packet sent!\n");
    }
}