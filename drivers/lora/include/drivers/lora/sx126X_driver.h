#pragma once

/**
 * REF: https://github.com/akshayabali/pico-lora
 * REF: https://github.com/sandeepmistry/arduino-LoRa
 */

#include <obc/api.h>
#include "drivers/gpio/gpio_driver.h"
#include "drivers/gpio/spi_driver.h"

/**
 * @brief Pinout data to use in the radio
 */
typedef struct SX126XPinout
{
    SPIInstance spi; /** SPI */
    PinNumber sck;   /** SCK */
    PinNumber miso;  /** MISO */
    PinNumber mosi;  /** MOSI */
    PinNumber cs;    /** CS */
    PinNumber reset; /** RESET */
} SX126XPinout;

/**
 * @brief LoRa radio configuration. All changes are done internally, so keep it just for reference!
 */
typedef struct SX126XData
{
    SX126XPinout pinout;     /** Pinout pointer */
    INT32 txPower;           /** Power */
    UINT64 frequency;        /** Frequency to work with */
    INT32 packetIndex;       /** Index of packet */
    BOOL implicitHeaderMode; /** Implicit header mode */
} SX126XData;

/**
 * @brief Initializes the radio
 *
 * @param data Radio data
 * @param pinout Pinout data
 * @param frequency Frequency to work with
 * @return Result code
 */
FUNCRESULT sx126XInit(SX126XData *data, SX126XPinout *pinout, UINT64 frequency);

/**
 * @brief Send data over LoRa
 *
 * @param data Radio data
 * @param buffer Buffer to send
 * @param size Size of buffer
 * @return Result code
 */
FUNCRESULT sx126XWriteBuffer(SX126XData *data, const BYTE *buffer, SIZE size);

/**
 * @brief Check if next byte of data is available
 *
 * @param data Radio data
 * @param available Pointer to BOOL to store result
 * @return Result code
 */
FUNCRESULT sx126XAvailable(SX126XData *data, BOOL *available);

/**
 * @brief Parse packet
 *
 * @param data Radio data
 * @param size Size of packet
 * @param packetLengthOut Pointer to SIZE to store packet length
 * @return Result code
 */
FUNCRESULT sx126XParsePacket(SX126XData *data, SIZE size, SIZE *packetLengthOut);

/**
 * @brief Read next byte of data
 *
 * @param data Radio data
 * @param dataOut Pointer to BYTE to store data
 * @return Result code
 */
FUNCRESULT sx126XRead(SX126XData *data, BYTE *dataOut);

/**
 * @brief Read next byte of data without advancing the packet read index
 *
 * @param data Radio data
 * @param dataOut Pointer to BYTE to store data
 * @return Result code
 */
FUNCRESULT sx126XPeek(SX126XData *data, BYTE *dataOut);

/**
 * @brief Receive data over LoRa
 *
 * @param data Radio data
 * @return Result code
 */
FUNCRESULT sx126XIdle(SX126XData *data);

/**
 * @brief Sleep the radio
 *
 * @param data Radio data
 * @return Result code
 */
FUNCRESULT sx126XSleep(SX126XData *data);

/**
 * @brief Get the RSSI of the last received packet
 *
 * @param data Radio data
 * @param rssi Pointer to INT32 to store RSSI
 * @return Result code
 */
FUNCRESULT sx126XPacketRssi(SX126XData *data, INT32 *rssi);

/**
 * @brief Get the SNR of the last received packet
 *
 * @param data Radio data
 * @param snr Pointer to FLOAT to store SNR
 * @return Result code
 */
FUNCRESULT sx126XPacketSnr(SX126XData *data, FLOAT *snr);

/**
 * @brief Get the frequency error of the last received packet
 *
 * @param data Radio data
 * @param error Pointer to INT64 to store frequency error
 * @return Result code
 */
FUNCRESULT sx126XPacketFrequencyError(SX126XData *data, INT64 *error);

/**
 * @brief Get the current RSSI
 *
 * @param data Radio data
 * @param rssi Pointer to INT32 to store RSSI
 * @return Result code
 */
FUNCRESULT sx126XRssi(SX126XData *data, INT32 *rssi);

/**
 * @brief Set the radio transmit power
 *
 * @param data Radio data
 * @param level Power level
 * @return Result code
 */
FUNCRESULT sx126XSetTxPower(SX126XData *data, INT32 level);

/**
 * @brief Set the radio frequency
 *
 * @param data Radio data
 * @param frequency Frequency to set
 * @return Result code
 */
FUNCRESULT sx126XSetFrequency(SX126XData *data, UINT64 frequency);

/**
 * @brief Set the radio spreading factor
 *
 * @param data Radio data
 * @param sf Spreading factor
 * @return Result code
 */
FUNCRESULT sx126XSetSpreadingFactor(SX126XData *data, INT32 sf);

/**
 * @brief Set the radio signal bandwidth
 *
 * @param data Radio data
 * @param sbw Signal bandwidth
 * @return Result code
 */
FUNCRESULT sx126XSetSignalBandwidth(SX126XData *data, INT64 sbw);

/**
 * @brief Set the radio coding rate
 *
 * @param data Radio data
 * @param denominator Coding rate denominator
 * @return Result code
 */
FUNCRESULT sx126XSetCodingRate4(SX126XData *data, INT32 denominator);

/**
 * @brief Set the radio preamble length
 *
 * @param data Radio data
 * @param length Preamble length
 * @return Result code
 */
FUNCRESULT sx126XSetPreambleLength(SX126XData *data, INT64 length);

/**
 * @brief Set the radio sync word
 *
 * @param data Radio data
 * @param sw Sync word
 * @return Result code
 */
FUNCRESULT sx126XSetSyncWord(SX126XData *data, INT32 sw);

/**
 * @brief Enable the radio CRC
 *
 * @param data Radio data
 * @return Result code
 */
FUNCRESULT sx126XEnableCrc(SX126XData *data);

/**
 * @brief Disable the radio CRC
 *
 * @param data Radio data
 * @return Result code
 */
FUNCRESULT sx126XDisableCrc(SX126XData *data);

/**
 * @brief Enable the radio invert IQ
 *
 * @param data Radio data
 * @return Result code
 */
FUNCRESULT sx126XEnableInvertIQ(SX126XData *data);

/**
 * @brief Disable the radio invert IQ
 *
 * @param data Radio data
 * @return Result code
 */
FUNCRESULT sx126XDisableInvertIQ(SX126XData *data);

/**
 * @brief Set the radio OCP
 *
 * @param data Radio data
 * @param mA OCP current
 * @return Result code
 */
FUNCRESULT sx126XSetOCP(SX126XData *data, BYTE mA);

/**
 * @brief Set the radio gain
 *
 * @param data Radio data
 * @param gain Gain
 * @return Result code
 */
FUNCRESULT sx126XSetGain(SX126XData *data, BYTE gain);

/**
 * @brief Set the explicit header mode
 *
 * @param data Radio data
 */
VOID __sx126XExplicitHeaderMode(SX126XData *data);

/**
 * @brief Set the implicit header mode
 *
 * @param data Radio data
 */
VOID __sx126XImplicitHeaderMode(SX126XData *data);

/**
 * @brief Check if the radio is transmitting
 *
 * @param data Radio data
 * @return TRUE if transmitting, FALSE otherwise
 */
BOOL __sx126XIsTransmitting(SX126XData *data);

/**
 * @brief Get the radio spreading factor
 *
 * @param data Radio data
 */
INT32 __sx126XGetSpreadingFactor(SX126XData *data);

/**
 * @brief Get the radio signal bandwidth
 *
 * @param data Radio data
 * @return Signal bandwidth
 */
INT64 __sx126XGetSignalBandwidth(SX126XData *data);

/**
 * @brief Set the radio LDO flag
 *
 * @param data Radio data
 */
VOID __sx126XSetLdoFlag(SX126XData *data);

/**
 * @brief Read a register from the radio
 *
 * @param data Radio data
 * @param address Register address
 * @return Register value
 */
BYTE __sx126XReadRegister(SX126XData *data, BYTE address);

/**
 * @brief Write a register to the radio
 *
 * @param data Radio data
 * @param address Register address
 * @param value Register value
 */
VOID __sx126XWriteRegister(SX126XData *data, BYTE address, BYTE value);

/**
 * @brief Trasnfer a byte to or from the radio
 *
 * @param data Radio data
 * @param address Register address
 * @param value Register value
 * @return Register value
 */
BYTE __sx126XSingleTransfer(SX126XData *data, BYTE address, BYTE value);