#ifndef _SX127X_DRIVER_H
#define _SX127X_DRIVER_H

/**
 * REF: https://github.com/akshayabali/pico-lora
 * REF: https://github.com/sandeepmistry/arduino-LoRa
 */

#include "hal/gpio_driver.h"
#include "hal/spi_driver.h"
#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

/**
 * @brief LoRa radio configuration. All changes are done internally, so keep it just for reference!
 */
typedef struct sx127x_data
{
    hal_spi_instance_t spi;       /** SPI Instance */
    hal_pin_number_t cs;          /** CS */
    hal_pin_number_t reset;       /** Reset pin */
    int txPower;                  /** Power */
    unsigned long long frequency; /** Frequency to work with */
    int packetIndex;              /** Index of packet */
    bool implicitHeaderMode;      /** Implicit header mode */
} sx127x_data_t;

/**
 * @brief Initializes the radio
 *
 * @param data Radio data
 * @param spi SPI Instance
 * @param cs CS
 * @param reset Reset
 * @param frequency Frequency to work with
 */
void sx127x_init(sx127x_data_t *data, hal_spi_instance_t spi, hal_pin_number_t cs, hal_pin_number_t reset, unsigned long long frequency);

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
 * @return Available
 */
bool sx127x_available(const sx127x_data_t *data);

/**
 * @brief Parse packet
 *
 * @param data Radio data
 * @param size Size of packet
 * @return Packet length
 */
size_t sx127x_parse_packet(sx127x_data_t *data, size_t size);

/**
 * @brief Read next byte of data
 *
 * @param data Radio data
 * @return Data
 */
uint8_t sx127x_read(sx127x_data_t *data);

/**
 * @brief Read next byte of data without advancing the packet read index
 *
 * @param data Radio data
 * @return Data byte
 */
uint8_t sx127x_peek(const sx127x_data_t *data);

/**
 * @brief Receive data over LoRa
 *
 * @param data Radio data
 */
void sx127x_idle(const sx127x_data_t *data);

/**
 * @brief Sleep the radio
 *
 * @param data Radio data
 */
void sx127x_sleep(const sx127x_data_t *data);

/**
 * @brief Get the RSSI of the last received packet
 *
 * @param data Radio data
 * @return RSSI
 */
int sx127x_packet_rssi(const sx127x_data_t *data);

/**
 * @brief Get the SNR of the last received packet
 *
 * @param data Radio data
 * @return SNR
 */
float sx127x_packet_snr(const sx127x_data_t *data);

/**
 * @brief Get the frequency error of the last received packet
 *
 * @param data Radio data
 * @return Frequency error
 */
long sx127x_packet_frequency_error(const sx127x_data_t *data);

/**
 * @brief Get the current RSSI
 *
 * @param data Radio data
 * @return RSSI
 */
int sx127x_rssi(const sx127x_data_t *data);

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
void sx127x_set_spreading_factor(const sx127x_data_t *data, int sf);

/**
 * @brief Set the radio signal bandwidth
 *
 * @param data Radio data
 * @param sbw Signal bandwidth
 */
void sx127x_set_signal_bandwidth(const sx127x_data_t *data, long sbw);

/**
 * @brief Set the radio coding rate
 *
 * @param data Radio data
 * @param denominator Coding rate denominator
 */
void sx127x_set_coding_rate4(const sx127x_data_t *data, int denominator);

/**
 * @brief Set the radio preamble length
 *
 * @param data Radio data
 * @param length Preamble length
 */
void sx127x_set_preamble_length(const sx127x_data_t *data, long length);

/**
 * @brief Set the radio sync word
 *
 * @param data Radio data
 * @param sw Sync word
 */
void sx127x_set_sync_word(const sx127x_data_t *data, int sw);

/**
 * @brief Enable the radio CRC
 *
 * @param data Radio data
 */
void sx127x_enable_crc(const sx127x_data_t *data);

/**
 * @brief Disable the radio CRC
 *
 * @param data Radio data
 */
void sx127x_disable_crc(const sx127x_data_t *data);

/**
 * @brief Enable the radio invert IQ
 *
 * @param data Radio data
 */
void sx127x_enable_invert_iq(const sx127x_data_t *data);

/**
 * @brief Disable the radio invert IQ
 *
 * @param data Radio data
 */
void sx127x_disable_invert_iq(const sx127x_data_t *data);

/**
 * @brief Set the radio OCP
 *
 * @param data Radio data
 * @param mA OCP current
 */
void sx127x_set_ocp(const sx127x_data_t *data, uint8_t mA);

/**
 * @brief Set the radio gain
 *
 * @param data Radio data
 * @param gain Gain
 */
void sx127x_set_gain(const sx127x_data_t *data, uint8_t gain);

#endif