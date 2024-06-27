#include "hal/board_control.h"
#include "hal/serial_driver.h"
#include "hal/time_tracker.h"
#include "hal/gpio_driver.h"
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

#define LED_UP_TIME_MS 20
#define LED_RX_UART_PIN 28
#define LED_RX_RADIO_PIN 5

#define LORA_FREQUENCY 433E6
#define LORA_BANDWIDTH 125E3
#define LORA_SF 8

#define UART 1
#define RX 9
#define TX 8
#define UART_BAUDRATE 115200

int main()
{
    hal_board_init(3000);

    hal_serial_printf("Initialized board!\n");
    hal_serial_printf("Firmware version: 1.0\n");
    hal_serial_printf("Author: Filip Gawlik\n");

    hal_spi_init_all(SPI, MISO, MOSI, SCK, SPI_FREQUENCY);
    hal_uart_init_all(UART, RX, TX, UART_BAUDRATE);

    msec_t uartLedOffset = 0, radioLedOffset = 0;
    hal_gpio_init_pin(LED_RX_UART_PIN, GPIO_OUTPUT);
    hal_gpio_init_pin(LED_RX_RADIO_PIN, GPIO_OUTPUT);

    sx127x_data_t loraData = {};
    sx127x_init(&loraData, SPI, CS, RESET, LORA_FREQUENCY);
    sx127x_set_signal_bandwidth(&loraData, LORA_BANDWIDTH);
    sx127x_set_spreading_factor(&loraData, LORA_SF);

    uint8_t curSize = 0;
    uint8_t recvBuffer[64];
    uint8_t bufLen = 0;

    while (true)
    {
        if (hal_uart_is_readable(UART))
        {
            uint8_t byte;
            hal_uart_read(UART, &byte, 1);

            if (curSize == 0)
            {
                if (byte <= sizeof(recvBuffer))
                {
                    curSize = byte;

                    hal_gpio_set_pin_state(LED_RX_UART_PIN, GPIO_HIGH);
                    uartLedOffset = hal_time_get_ms_since_boot();

                    hal_serial_printf("Received request for %d bytes\n", curSize);
                }
            }
            else
            {
                recvBuffer[bufLen++] = byte;

                if (bufLen == curSize)
                {
                    hal_serial_printf("All bytes received!\n");

                    sx127x_write_buffer(&loraData, recvBuffer, bufLen);

                    hal_serial_printf("Packet sent!\n");

                    curSize = 0;
                    bufLen = 0;
                }
            }
        }

        size_t packetSize = sx127x_parse_packet(&loraData, 0);

        if (packetSize > 0)
        {
            hal_serial_printf("Received packet with size: %d\n", packetSize);

            uint8_t buffer[256];
            uint8_t i = 0;

            while (sx127x_available(&loraData))
            {
                if (i == packetSize)
                {
                    hal_serial_printf("Something went wrong while parsing packet!");

                    break;
                }

                if (i < sizeof(buffer))
                {
                    buffer[i++] = sx127x_read(&loraData);
                }
                else
                {
                    hal_serial_printf("Buffer overflow while parsing packet!");

                    break;
                }
            }

            hal_uart_write(UART, &i, 1);
            hal_uart_write(UART, buffer, i);

            hal_gpio_set_pin_state(LED_RX_RADIO_PIN, GPIO_HIGH);
            radioLedOffset = hal_time_get_ms_since_boot();
        }

        if (uartLedOffset != 0 && hal_time_get_ms_since_boot() - uartLedOffset >= LED_UP_TIME_MS)
        {
            hal_gpio_set_pin_state(LED_RX_UART_PIN, GPIO_LOW);
            uartLedOffset = 0;
        }

        if (radioLedOffset != 0 && hal_time_get_ms_since_boot() - radioLedOffset >= LED_UP_TIME_MS)
        {
            hal_gpio_set_pin_state(LED_RX_RADIO_PIN, GPIO_LOW);
            radioLedOffset = 0;
        }
    }
}