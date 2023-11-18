#ifndef _SX127X_DRIVER_H
#define _SX127X_DRIVER_H

/**
 * REF: https://github.com/akshayabali/pico-lora
 * REF: https://github.com/sandeepmistry/arduino-LoRa
 */

#include "modules/drivers/hal/gpio_driver.h"
#include "modules/drivers/hal/spi_driver.h"
#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

/**
 * @brief Pinout data to use in the radio
 */
typedef struct sx127x_pinout
{
    spi_instance_t spi; /** SPI */
    pin_number_t sck;   /** SCK */
    pin_number_t miso;  /** MISO */
    pin_number_t mosi;  /** MOSI */
    pin_number_t cs;    /** CS */
    pin_number_t reset; /** RESET */
} sx127x_pinout_t;

/**
 * @brief LoRa radio configuration. All changes are done internally, so keep it just for reference!
 */
typedef struct sx127x_data
{
    sx127x_pinout_t pinout;       /** Pinout pointer */
    int txPower;                  /** Power */
    unsigned long long frequency; /** Frequency to work with */
    int packetIndex;              /** Index of packet */
    bool implicitHeaderMode;      /** Implicit header mode */
} sx127x_data_t;

/**
 * @brief Initializes the radio
 *
 * @param data Radio data
 * @param pinout Pinout data
 * @param frequency Frequency to work with
 */
void sx127x_init(sx127x_data_t *data, sx127x_pinout_t *pinout, unsigned long long frequency);

/**
 * @brief Send data over LoRa
 *
 * @param data Radio data
 * @param buffer Buffer to send
 * @param size Size of buffer
 */
void sx127x_write_buffer(sx127x_data_t *data, const uint8_t *buffer, size_t size);

/**
 * @brief Check if next byte of data is available
 *
 * @param data Radio data
 * @param available Pointer to bool to store result
 */
void sx127x_available(sx127x_data_t *data, bool *available);

/**
 * @brief Parse packet
 *
 * @param data Radio data
 * @param size Size of packet
 * @param packetLengthOut Pointer to size_t to store packet length
 */
void sx127x_parse_packet(sx127x_data_t *data, size_t size, size_t *packetLengthOut);

/**
 * @brief Read next byte of data
 *
 * @param data Radio data
 * @param dataOut Pointer to uint8_t to store data
 */
void sx127x_read(sx127x_data_t *data, uint8_t *dataOut);

/**
 * @brief Read next byte of data without advancing the packet read index
 *
 * @param data Radio data
 * @param dataOut Pointer to uint8_t to store data
 */
void sx127x_peek(sx127x_data_t *data, uint8_t *dataOut);

/**
 * @brief Receive data over LoRa
 *
 * @param data Radio data
 */
void sx127x_idle(sx127x_data_t *data);

/**
 * @brief Sleep the radio
 *
 * @param data Radio data
 */
void sx127x_sleep(sx127x_data_t *data);

/**
 * @brief Get the RSSI of the last received packet
 *
 * @param data Radio data
 * @param rssi Pointer to int to store RSSI
 */
void sx127x_packet_rssi(sx127x_data_t *data, int *rssi);

/**
 * @brief Get the SNR of the last received packet
 *
 * @param data Radio data
 * @param snr Pointer to float to store SNR
 */
void sx127x_packet_snr(sx127x_data_t *data, float *snr);

/**
 * @brief Get the frequency error of the last received packet
 *
 * @param data Radio data
 * @param error Pointer to long to store frequency error
 */
void sx127x_packet_frequency_error(sx127x_data_t *data, long *error);

/**
 * @brief Get the current RSSI
 *
 * @param data Radio data
 * @param rssi Pointer to int to store RSSI
 */
void sx127x_rssi(sx127x_data_t *data, int *rssi);

/**
 * @brief Set the radio transmit power
 *
 * @param data Radio data
 * @param level Power level
 */
void sx127x_set_tx_power(sx127x_data_t *data, int level);

/**
 * @brief Set the radio frequency
 *
 * @param data Radio data
 * @param frequency Frequency to set
 */
void sx127x_set_frequency(sx127x_data_t *data, unsigned long long frequency);

/**
 * @brief Set the radio spreading factor
 *
 * @param data Radio data
 * @param sf Spreading factor
 */
void sx127x_set_spreading_factor(sx127x_data_t *data, int sf);

/**
 * @brief Set the radio signal bandwidth
 *
 * @param data Radio data
 * @param sbw Signal bandwidth
 */
void sx127x_set_signal_bandwidth(sx127x_data_t *data, long sbw);

/**
 * @brief Set the radio coding rate
 *
 * @param data Radio data
 * @param denominator Coding rate denominator
 */
void sx127x_set_coding_rate4(sx127x_data_t *data, int denominator);

/**
 * @brief Set the radio preamble length
 *
 * @param data Radio data
 * @param length Preamble length
 */
void sx127x_set_preamble_length(sx127x_data_t *data, long length);

/**
 * @brief Set the radio sync word
 *
 * @param data Radio data
 * @param sw Sync word
 */
void sx127x_set_sync_word(sx127x_data_t *data, int sw);

/**
 * @brief Enable the radio CRC
 *
 * @param data Radio data
 */
void sx127x_enable_crc(sx127x_data_t *data);

/**
 * @brief Disable the radio CRC
 *
 * @param data Radio data
 */
void sx127x_disable_crc(sx127x_data_t *data);

/**
 * @brief Enable the radio invert IQ
 *
 * @param data Radio data
 */
void sx127x_enable_invert_i_q(sx127x_data_t *data);

/**
 * @brief Disable the radio invert IQ
 *
 * @param data Radio data
 */
void sx127x_disable_invert_i_q(sx127x_data_t *data);

/**
 * @brief Set the radio OCP
 *
 * @param data Radio data
 * @param mA OCP current
 */
void sx127x_set_o_c_p(sx127x_data_t *data, uint8_t mA);

/**
 * @brief Set the radio gain
 *
 * @param data Radio data
 * @param gain Gain
 */
void sx127x_set_gain(sx127x_data_t *data, uint8_t gain);

/**
 * @brief Set the explicit header mode
 *
 * @param data Radio data
 */
void _sx127x_explicit_header_mode(sx127x_data_t *data);

/**
 * @brief Set the implicit header mode
 *
 * @param data Radio data
 */
void _sx127x_implicit_header_mode(sx127x_data_t *data);

/**
 * @brief Check if the radio is transmitting
 *
 * @param data Radio data
 * @return true if transmitting, false otherwise
 */
bool _sx127x_is_transmitting(sx127x_data_t *data);

/**
 * @brief Get the radio spreading factor
 *
 * @param data Radio data
 */
int _sx127XGetSpreadingFactor(sx127x_data_t *data);

/**
 * @brief Get the radio signal bandwidth
 *
 * @param data Radio data
 * @return Signal bandwidth
 */
long _sx127XGetSignalBandwidth(sx127x_data_t *data);

/**
 * @brief Set the radio LDO flag
 *
 * @param data Radio data
 */
void _sx127x_set_ldo_flag(sx127x_data_t *data);

/**
 * @brief Read a register from the radio
 *
 * @param data Radio data
 * @param address Register address
 * @return Register value
 */
uint8_t _sx127x_read_register(sx127x_data_t *data, uint8_t address);

/**
 * @brief Write a register to the radio
 *
 * @param data Radio data
 * @param address Register address
 * @param value Register value
 */
void _sx127x_write_register(sx127x_data_t *data, uint8_t address, uint8_t value);

/**
 * @brief Trasnfer a byte to or from the radio
 *
 * @param data Radio data
 * @param address Register address
 * @param value Register value
 * @return Register value
 */
uint8_t _sx127x_single_transfer(sx127x_data_t *data, uint8_t address, uint8_t value);

#endif