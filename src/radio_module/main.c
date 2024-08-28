#include "hal/board_control.h"
#include "hal/serial_driver.h"
#include "hal/time_tracker.h"
#include "hal/gpio_driver.h"
#include "hal/uart_driver.h"
#include "hal/spi_driver.h"
#include "lib/drivers/lora/sx127X_driver.h"
#include "lib/drivers/lora/sx126X_driver.h"

#define SPI 0
#define CS 5
#define MISO 4
#define MOSI 3
#define SCK 2
#define RESET 1
#define BUSY 0
#define RXEN 27
#define TXEN 28
#define SPI_FREQUENCY 100000

#define LORA_FREQUENCY 433E6

#define UART 0
#define RX 13
#define TX 12
#define UART_BAUDRATE 115200

int main()
{
    hal_board_init(1000);

    hal_serial_printf("Initialized board!\n");
    hal_serial_printf("Firmware version: 1.0\n");
    hal_serial_printf("Author: Filip Gawlik\n");

    hal_spi_init_all(SPI, MISO, MOSI, SCK, SPI_FREQUENCY);
    hal_uart_init_all(UART, RX, TX, UART_BAUDRATE);

    hal_gpio_init_pin(RXEN, GPIO_OUTPUT);
    hal_gpio_init_pin(TXEN, GPIO_OUTPUT);
    hal_gpio_set_pin_state(RXEN, GPIO_LOW);
    hal_gpio_set_pin_state(TXEN, GPIO_HIGH);

    sx126x_config_t loraConfig = {};
    sx126x_init(&loraConfig, SPI, CS, RESET, BUSY);
    sx126x_set_standby(&loraConfig, SX126X_STANDBY_RC);
    sx126x_clear_irq_status(&loraConfig, SX126X_IRQ_RADIO_ALL_MASK);
    sx126x_set_dio_irq_params(&loraConfig, SX126X_IRQ_RADIO_ALL_MASK, SX126X_IRQ_TX_DONE_MASK | SX126X_IRQ_RX_TX_TIMEOUT_MASK | SX126X_IRQ_RX_DONE_MASK, SX126X_IRQ_NONE, SX126X_IRQ_NONE);
    sx126x_set_dio2_as_rf_switch_ctrl(&loraConfig, true);
    sx126x_set_dio3_as_tcxo_ctrl(&loraConfig, SX126X_TCXO_CTRL_3_3V, 100);
    sx126x_set_regulator_mode(&loraConfig, SX126X_REGULATOR_DC_DC);
    sx126x_set_pa_config(&loraConfig, 0x04, 0x07);
    sx126x_calibrate(&loraConfig, SX126X_CALIBRATE_ALL_MASK);
    sx126x_calibrate_image(&loraConfig, LORA_FREQUENCY);
    sx126x_set_rf_frequency(&loraConfig, LORA_FREQUENCY);
    sx126x_set_packet_type(&loraConfig, SX126X_PACKET_TYPE_LORA);
    sx126x_set_lora_modulation_params(&loraConfig, SX126X_LORA_SF_8, SX126X_LORA_BW_125, SX126X_LORA_CR_4_5, 0x00);
    sx126x_set_packet_lora_params(&loraConfig, 16, SX126X_LORA_HEADER_EXPLICIT, 255, true, false);
    sx126x_set_buffer_base_address(&loraConfig, 0, 0);
    sx126x_set_tx_params(&loraConfig, 22, SX126X_RAMP_10U);

    bool rx = false;

    uint8_t curSize = 0;
    uint8_t recvBuffer[64];
    uint8_t bufLen = 0;

    while (true)
    {
        while (hal_uart_is_readable(UART))
        {
            uint8_t byte;
            hal_uart_read(UART, &byte, 1);

            if (curSize == 0)
            {
                if (byte <= sizeof(recvBuffer))
                {
                    curSize = byte;

                    rx = false;

                    hal_serial_printf("Received request for %d bytes\n", curSize);
                }
                else
                {
                    hal_serial_printf("Received invalid number of bytes (%d)\n", byte);
                }
            }
            else
            {
                recvBuffer[bufLen++] = byte;

                if (bufLen == curSize)
                {
                    hal_serial_printf("All bytes received!\n");

                    sx126x_set_standby(&loraConfig, SX126X_STANDBY_RC);
                    sx126x_set_buffer_base_address(&loraConfig, 0, 0);
                    sx126x_write_buffer(&loraConfig, 0, recvBuffer, bufLen);
                    sx126x_set_packet_lora_params(&loraConfig, 16, SX126X_LORA_HEADER_EXPLICIT, bufLen, true, false);
                    sx126x_set_tx(&loraConfig, 0);

                    hal_serial_printf("Packet sent!\n");

                    rx = true;

                    curSize = 0;
                    bufLen = 0;
                }
            }
        }

        if (rx)
        {
            // size_t packetSize = sx127x_parse_packet(&loraData, 0);

            // if (packetSize > 0)
            // {
            //     hal_serial_printf("Received packet with size: %d\n", packetSize);

            //     uint8_t buffer[256];
            //     uint8_t i = 0;

            //     while (sx127x_available(&loraData))
            //     {
            //         if (i == packetSize)
            //         {
            //             hal_serial_printf("Something went wrong while parsing packet!");

            //             break;
            //         }

            //         if (i < sizeof(buffer))
            //         {
            //             buffer[i++] = sx127x_read(&loraData);
            //         }
            //         else
            //         {
            //             hal_serial_printf("Buffer overflow while parsing packet!");

            //             break;
            //         }
            //     }

            //     hal_uart_write(UART, &i, 1);
            //     hal_uart_write(UART, buffer, i);
            // }
        }
    }
}