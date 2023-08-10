#pragma once

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
typedef struct SX126XConfig
{
    SX126XPinout pinout;     /** Pinout pointer */
    INT32 txPower;           /** Power */
    UINT64 frequency;        /** Frequency to work with */
    INT32 packetIndex;       /** Index of packet */
    BOOL implicitHeaderMode; /** Implicit header mode */
} SX126XConfig;

typedef enum SX126XIRQ
{
    SX126X_IRQ_TX_DONE = 0x00,
    SX126X_IRQ_RX_DONE = 0x01,
    SX126X_IRQ_PREAMBLE_DETECTED = 0x02,
    SX126X_IRQ_SYNC_WORD_VALID = 0x03,
    SX126X_IRQ_HEADER_VALID = 0x04,
    SX126X_IRQ_HEADER_ERR = 0x05,
    SX126X_IRQ_CRC_ERR = 0x06,
    SX126X_IRQ_CAD_DONE = 0x07,
    SX126X_IRQ_CAD_DETECTED = 0x08,
    SX126X_IRQ_TIMEOUT = 0x09,
} SX126XIRQ;

typedef enum SX126XOpMode
{
    SX126X_OPMODE_SLEEP = 0x00,
    SX126X_OPMODE_STDBY_RC = 0x01,
    SX126X_OPMODE_STDBY_XOSC = 0x02,
    SX126X_OPMODE_FS = 0x03,
    SX126X_OPMODE_TX = 0x04,
    SX126X_OPMODE_RX = 0x05,
} SX126XOpMode;

FUNCRESULT sx126XInit(SX126XConfig *data, SX126XPinout *pinout, UINT64 frequency);
FUNCRESULT sx126XSetSleep(SX126XConfig *data, BOOL coldStart);
FUNCRESULT sx126XSetStandby(SX126XConfig *data, BOOL rc);
VOID __sx126XWriteRegister(SX126XConfig *data, UINT16 address, BYTE *buffer, SIZE szBuffer);
VOID __sx126XReadRegister(SX126XConfig *data, UINT16 address, BYTE* buffer, SIZE szBuffer);
VOID __sx126XCMD(SX126XConfig *data, BYTE command, BYTE *params, SIZE szParams, BYTE *resultBuffer, SIZE szBuffer);