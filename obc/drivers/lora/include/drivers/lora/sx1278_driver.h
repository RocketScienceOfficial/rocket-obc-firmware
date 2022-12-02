#pragma once

#include <obc/api.h>
#include "drivers/gpio/gpio_driver.h"
#include "drivers/gpio/spi_driver.h"

/**
 * @brief Pinout data to use in the radio.
 */
typedef struct SX1278Pinout
{
    SPIInstance spi;
    PinNumber sck;
    PinNumber miso;
    PinNumber mosi;
    PinNumber cs;
    PinNumber ss;
    PinNumber reset;
} SX1278Pinout;

/**
 * @brief LoRa radio configuration. All changes are done internally, so keep it just for reference!
 */
typedef struct SX1278Data
{
    SX1278Pinout _pinout;
    INT32 _txPower;
    UINT64 _frequency;
    INT32 _packetIndex;
    BOOL _implicitHeaderMode;
} SX1278Data;

FUNCRESULT sx1278Init(SX1278Data *data, SX1278Pinout *pinout, UINT64 frequency);

FUNCRESULT sx1278WriteBuffer(SX1278Data *data, const BYTE *buffer, SIZE size);
FUNCRESULT sx1278Available(SX1278Data *data, BOOL *available);
FUNCRESULT sx1278ParsePacket(SX1278Data *data, SIZE size, SIZE* packetLengthOut);
FUNCRESULT sx1278Read(SX1278Data *data, BYTE *dataOut);
FUNCRESULT sx1278Peek(SX1278Data *data, BYTE *dataOut);
FUNCRESULT sx1278Idle(SX1278Data *data);
FUNCRESULT sx1278Sleep(SX1278Data *data);

FUNCRESULT sx1278PacketRssi(SX1278Data *data, INT32 *rssi);
FUNCRESULT sx1278PacketSnr(SX1278Data *data, FLOAT *snr);
FUNCRESULT sx1278PacketFrequencyError(SX1278Data *data, INT64 *error);
FUNCRESULT sx1278Rssi(SX1278Data *data, INT32 *rssi);

FUNCRESULT sx1278SetTxPower(SX1278Data *data, INT32 level);
FUNCRESULT sx1278SetFrequency(SX1278Data *data, UINT64 frequency);
FUNCRESULT sx1278SetSpreadingFactor(SX1278Data *data, INT32 sf);
FUNCRESULT sx1278SetSignalBandwidth(SX1278Data *data, INT64 sbw);
FUNCRESULT sx1278SetCodingRate4(SX1278Data *data, INT32 denominator);
FUNCRESULT sx1278SetPreambleLength(SX1278Data *data, INT64 length);
FUNCRESULT sx1278SetSyncWord(SX1278Data *data, INT32 sw);
FUNCRESULT sx1278EnableCrc(SX1278Data *data);
FUNCRESULT sx1278DisableCrc(SX1278Data *data);
FUNCRESULT sx1278EnableInvertIQ(SX1278Data *data);
FUNCRESULT sx1278DisableInvertIQ(SX1278Data *data);
FUNCRESULT sx1278SetOCP(SX1278Data *data, BYTE mA);
FUNCRESULT sx1278SetGain(SX1278Data *data, BYTE gain);

VOID __sx1278ExplicitHeaderMode(SX1278Data *data);
VOID __sx1278ImplicitHeaderMode(SX1278Data *data);
BOOL __sx1278IsTransmitting(SX1278Data *data);
INT32 __sx1278GetSpreadingFactor(SX1278Data *data);
INT64 __sx1278GetSignalBandwidth(SX1278Data *data);
VOID __sx1278SetLdoFlag(SX1278Data *data);
BYTE __sx1278ReadRegister(SX1278Data *data, BYTE address);
VOID __sx1278WriteRegister(SX1278Data *data, BYTE address, BYTE value);
BYTE __sx1278SingleTransfer(SX1278Data *data, BYTE address, BYTE value);