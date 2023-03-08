#include "drivers/lora/sx1278_driver.h"
#include "pico/stdlib.h"
#include <string.h>

#define REG_FIFO 0x00
#define REG_OP_MODE 0x01
#define REG_FRF_MSB 0x06
#define REG_FRF_MID 0x07
#define REG_FRF_LSB 0x08
#define REG_PA_CONFIG 0x09
#define REG_OCP 0x0b
#define REG_LNA 0x0c
#define REG_FIFO_ADDR_PTR 0x0d
#define REG_FIFO_TX_BASE_ADDR 0x0e
#define REG_FIFO_RX_BASE_ADDR 0x0f
#define REG_FIFO_RX_CURRENT_ADDR 0x10
#define REG_IRQ_FLAGS 0x12
#define REG_RX_NB_BYTES 0x13
#define REG_PKT_SNR_VALUE 0x19
#define REG_PKT_RSSI_VALUE 0x1a
#define REG_RSSI_VALUE 0x1b
#define REG_MODEM_CONFIG_1 0x1d
#define REG_MODEM_CONFIG_2 0x1e
#define REG_PREAMBLE_MSB 0x20
#define REG_PREAMBLE_LSB 0x21
#define REG_PAYLOAD_LENGTH 0x22
#define REG_MODEM_CONFIG_3 0x26
#define REG_FREQ_ERROR_MSB 0x28
#define REG_FREQ_ERROR_MID 0x29
#define REG_FREQ_ERROR_LSB 0x2a
#define REG_RSSI_WIDEBAND 0x2c
#define REG_DETECTION_OPTIMIZE 0x31
#define REG_INVERTIQ 0x33
#define REG_DETECTION_THRESHOLD 0x37
#define REG_SYNC_WORD 0x39
#define REG_INVERTIQ2 0x3b
#define REG_DIO_MAPPING_1 0x40
#define REG_VERSION 0x42
#define REG_PA_DAC 0x4d

#define MODE_LONG_RANGE_MODE 0x80
#define MODE_SLEEP 0x00
#define MODE_STDBY 0x01
#define MODE_TX 0x03
#define MODE_RX_CONTINUOUS 0x05
#define MODE_RX_SINGLE 0x06

#define PA_BOOST 0x80
#define PA_OUTPUT_RFO_PIN 0
#define PA_OUTPUT_PA_BOOST_PIN 1

#define IRQ_TX_DONE_MASK 0x08
#define IRQ_PAYLOAD_CRC_ERROR_MASK 0x20
#define IRQ_RX_DONE_MASK 0x40

#define RF_MID_BAND_THRESHOLD 525E6
#define RSSI_OFFSET_HF_PORT 157
#define RSSI_OFFSET_LF_PORT 164

#define MAX_PKT_LENGTH 255

#define LORA_DEFAULT_TX_POWER 17

FUNCRESULT sx1278Init(SX1278Data *data, SX1278Pinout *pinout, UINT64 frequency)
{
    if (!data || !pinout)
    {
        return ERR_INVALIDARG;
    }

    data->_pinout = *pinout;
    data->_txPower = LORA_DEFAULT_TX_POWER;
    data->_frequency = frequency;
    data->_packetIndex = 0;
    data->_implicitHeaderMode = 0;

    gpioInitPin(data->_pinout.ss, GPIO_OUTPUT);
    gpioSetPinState(data->_pinout.ss, GPIO_HIGH);

    if (data->_pinout.reset != -1)
    {
        gpioInitPin(data->_pinout.reset, GPIO_OUTPUT);

        gpioSetPinState(data->_pinout.reset, GPIO_LOW);
        sleep_ms(10);

        gpioSetPinState(data->_pinout.reset, GPIO_HIGH);
        sleep_ms(10);
    }

    spiInitPins(data->_pinout.spi, data->_pinout.miso, data->_pinout.mosi, data->_pinout.sck, data->_pinout.cs);

    BYTE version = __sx1278ReadRegister(data, REG_VERSION);

    if (version != 0x12)
    {
        return ERR_UNEXPECTED;
    }

    sx1278Sleep(data);

    sx1278SetFrequency(data, data->_frequency);
    sx1278SetTxPower(data, data->_txPower);

    __sx1278WriteRegister(data, REG_FIFO_TX_BASE_ADDR, 0);
    __sx1278WriteRegister(data, REG_FIFO_RX_BASE_ADDR, 0);
    __sx1278WriteRegister(data, REG_LNA, __sx1278ReadRegister(data, REG_LNA) | 0x03);
    __sx1278WriteRegister(data, REG_MODEM_CONFIG_3, 0x04);

    sx1278Idle(data);

    return SUC_OK;
}

FUNCRESULT sx1278WriteBuffer(SX1278Data *data, const BYTE *buffer, SIZE size)
{
    if (__sx1278IsTransmitting(data))
    {
        return ERR_ACCESSDENIED;
    }

    sx1278Idle(data);

    __sx1278ExplicitHeaderMode(data);
    __sx1278WriteRegister(data, REG_FIFO_ADDR_PTR, 0);
    __sx1278WriteRegister(data, REG_PAYLOAD_LENGTH, 0);

    if (size > MAX_PKT_LENGTH)
    {
        size = MAX_PKT_LENGTH;
    }

    for (SIZE i = 0; i < size; i++)
    {
        __sx1278WriteRegister(data, REG_FIFO, buffer[i]);
    }

    __sx1278WriteRegister(data, REG_PAYLOAD_LENGTH, size);
    __sx1278WriteRegister(data, REG_OP_MODE, MODE_LONG_RANGE_MODE | MODE_TX);

    while ((__sx1278ReadRegister(data, REG_IRQ_FLAGS) & IRQ_TX_DONE_MASK) == 0)
    {
        sleep_ms(0);
    }

    __sx1278WriteRegister(data, REG_IRQ_FLAGS, IRQ_TX_DONE_MASK);

    return SUC_OK;
}

FUNCRESULT sx1278Available(SX1278Data *data, BOOL *available)
{
    *available = (__sx1278ReadRegister(data, REG_RX_NB_BYTES) - data->_packetIndex);

    return SUC_OK;
}

FUNCRESULT sx1278ParsePacket(SX1278Data *data, SIZE size, SIZE *packetLengthOut)
{
    SIZE packetLength = 0;
    INT32 irqFlags = __sx1278ReadRegister(data, REG_IRQ_FLAGS);

    if (size > 0)
    {
        __sx1278ImplicitHeaderMode(data);
        __sx1278WriteRegister(data, REG_PAYLOAD_LENGTH, size & 0xff);
    }
    else
    {
        __sx1278ExplicitHeaderMode(data);
    }

    __sx1278WriteRegister(data, REG_IRQ_FLAGS, irqFlags);
    __sx1278WriteRegister(data, REG_IRQ_FLAGS, irqFlags);

    if ((irqFlags & IRQ_RX_DONE_MASK) && (irqFlags & IRQ_PAYLOAD_CRC_ERROR_MASK) == 0)
    {
        data->_packetIndex = 0;

        if (data->_implicitHeaderMode)
        {
            packetLength = __sx1278ReadRegister(data, REG_PAYLOAD_LENGTH);
        }
        else
        {
            packetLength = __sx1278ReadRegister(data, REG_RX_NB_BYTES);
        }

        __sx1278WriteRegister(data, REG_FIFO_ADDR_PTR, __sx1278ReadRegister(data, REG_FIFO_RX_CURRENT_ADDR));

        sx1278Idle(data);
    }
    else if (__sx1278ReadRegister(data, REG_OP_MODE) != (MODE_LONG_RANGE_MODE | MODE_RX_SINGLE))
    {
        __sx1278WriteRegister(data, REG_FIFO_ADDR_PTR, 0);
        __sx1278WriteRegister(data, REG_OP_MODE, MODE_LONG_RANGE_MODE | MODE_RX_SINGLE);
    }

    *packetLengthOut = packetLength;

    return SUC_OK;
}

FUNCRESULT sx1278Read(SX1278Data *data, BYTE *dataOut)
{
    BOOL available = FALSE;

    sx1278Available(data, &available);

    if (!available)
    {
        return ERR_ACCESSDENIED;
    }

    data->_packetIndex++;

    *dataOut = __sx1278ReadRegister(data, REG_FIFO);

    return SUC_OK;
}

FUNCRESULT sx1278Peek(SX1278Data *data, BYTE *dataOut)
{
    BOOL available = FALSE;

    sx1278Available(data, &available);

    if (!available)
    {
        return ERR_ACCESSDENIED;
    }

    BYTE currentAddress = __sx1278ReadRegister(data, REG_FIFO_ADDR_PTR);

    *dataOut = __sx1278ReadRegister(data, REG_FIFO);

    __sx1278WriteRegister(data, REG_FIFO_ADDR_PTR, currentAddress);

    return SUC_OK;
}

FUNCRESULT sx1278Idle(SX1278Data *data)
{
    __sx1278WriteRegister(data, REG_OP_MODE, MODE_LONG_RANGE_MODE | MODE_STDBY);

    return SUC_OK;
}

FUNCRESULT sx1278Sleep(SX1278Data *data)
{
    __sx1278WriteRegister(data, REG_OP_MODE, MODE_LONG_RANGE_MODE | MODE_SLEEP);

    return SUC_OK;
}

FUNCRESULT sx1278PacketRssi(SX1278Data *data, INT32 *rssi)
{
    *rssi = (__sx1278ReadRegister(data, REG_PKT_RSSI_VALUE) - (data->_frequency < RF_MID_BAND_THRESHOLD ? RSSI_OFFSET_LF_PORT : RSSI_OFFSET_HF_PORT));

    return SUC_OK;
}

FUNCRESULT sx1278PacketSnr(SX1278Data *data, FLOAT *snr)
{
    *snr = ((UINT8)__sx1278ReadRegister(data, REG_PKT_SNR_VALUE)) * 0.25;

    return SUC_OK;
}

FUNCRESULT sx1278PacketFrequencyError(SX1278Data *data, INT64 *error)
{
    INT32 freqError = 0;
    freqError = (INT32)(__sx1278ReadRegister(data, REG_FREQ_ERROR_MSB) & 0x111);
    freqError <<= 8L;
    freqError += (INT32)(__sx1278ReadRegister(data, REG_FREQ_ERROR_MID));
    freqError <<= 8L;
    freqError += (INT32)(__sx1278ReadRegister(data, REG_FREQ_ERROR_LSB));

    if (__sx1278ReadRegister(data, REG_FREQ_ERROR_MSB) & 0x1000)
    {
        freqError -= 524288;
    }

    const FLOAT fXtal = 32E6;
    const FLOAT fError = (((FLOAT)(freqError) * (1L << 24)) / fXtal) * (__sx1278GetSignalBandwidth(data) / 500000.0f);

    *error = (long)(fError);

    return SUC_OK;
}

FUNCRESULT sx1278Rssi(SX1278Data *data, INT32 *rssi)
{
    *rssi = (__sx1278ReadRegister(data, REG_RSSI_VALUE) - (data->_frequency < RF_MID_BAND_THRESHOLD ? RSSI_OFFSET_LF_PORT : RSSI_OFFSET_HF_PORT));

    return SUC_OK;
}

FUNCRESULT sx1278SetTxPower(SX1278Data *data, INT32 level)
{
    INT32 outputPin = PA_OUTPUT_PA_BOOST_PIN;

    if (outputPin == PA_OUTPUT_RFO_PIN)
    {
        if (level < 0)
        {
            level = 0;
        }
        else if (level > 14)
        {
            level = 14;
        }

        __sx1278WriteRegister(data, REG_PA_CONFIG, 0x70 | level);
    }
    else
    {
        if (level > 17)
        {
            if (level > 20)
            {
                level = 20;
            }

            level -= 3;

            __sx1278WriteRegister(data, REG_PA_DAC, 0x87);
            sx1278SetOCP(data, 140);
        }
        else
        {
            if (level < 2)
            {
                level = 2;
            }

            __sx1278WriteRegister(data, REG_PA_DAC, 0x84);
            sx1278SetOCP(data, 100);
        }

        __sx1278WriteRegister(data, REG_PA_CONFIG, PA_BOOST | (level - 2));
    }

    data->_txPower = level;

    return SUC_OK;
}

FUNCRESULT sx1278SetFrequency(SX1278Data *data, UINT64 frequency)
{
    data->_frequency = frequency;

    UINT64 frf = (frequency << 19) / 32000000;

    __sx1278WriteRegister(data, REG_FRF_MSB, (BYTE)(frf >> 16));
    __sx1278WriteRegister(data, REG_FRF_MID, (BYTE)(frf >> 8));
    __sx1278WriteRegister(data, REG_FRF_LSB, (BYTE)(frf >> 0));

    return SUC_OK;
}

FUNCRESULT sx1278SetSpreadingFactor(SX1278Data *data, INT32 sf)
{
    if (sf < 6)
    {
        sf = 6;
    }
    else if (sf > 12)
    {
        sf = 12;
    }

    if (sf == 6)
    {
        __sx1278WriteRegister(data, REG_DETECTION_OPTIMIZE, 0xc5);
        __sx1278WriteRegister(data, REG_DETECTION_THRESHOLD, 0x0c);
    }
    else
    {
        __sx1278WriteRegister(data, REG_DETECTION_OPTIMIZE, 0xc3);
        __sx1278WriteRegister(data, REG_DETECTION_THRESHOLD, 0x0a);
    }

    __sx1278WriteRegister(data, REG_MODEM_CONFIG_2, (__sx1278ReadRegister(data, REG_MODEM_CONFIG_2) & 0x0f) | ((sf << 4) & 0xf0));
    __sx1278SetLdoFlag(data);

    return SUC_OK;
}

FUNCRESULT sx1278SetSignalBandwidth(SX1278Data *data, INT64 sbw)
{
    INT32 bw;

    if (sbw <= 7.8E3)
    {
        bw = 0;
    }
    else if (sbw <= 10.4E3)
    {
        bw = 1;
    }
    else if (sbw <= 15.6E3)
    {
        bw = 2;
    }
    else if (sbw <= 20.8E3)
    {
        bw = 3;
    }
    else if (sbw <= 31.25E3)
    {
        bw = 4;
    }
    else if (sbw <= 41.7E3)
    {
        bw = 5;
    }
    else if (sbw <= 62.5E3)
    {
        bw = 6;
    }
    else if (sbw <= 125E3)
    {
        bw = 7;
    }
    else if (sbw <= 250E3)
    {
        bw = 8;
    }
    else
    {
        bw = 9;
    }

    __sx1278WriteRegister(data, REG_MODEM_CONFIG_1, (__sx1278ReadRegister(data, REG_MODEM_CONFIG_1) & 0x0f) | (bw << 4));
    __sx1278SetLdoFlag(data);

    return SUC_OK;
}

FUNCRESULT sx1278SetCodingRate4(SX1278Data *data, INT32 denominator)
{
    if (denominator < 5)
    {
        denominator = 5;
    }
    else if (denominator > 8)
    {
        denominator = 8;
    }

    INT32 cr = denominator - 4;

    __sx1278WriteRegister(data, REG_MODEM_CONFIG_1, (__sx1278ReadRegister(data, REG_MODEM_CONFIG_1) & 0xf1) | (cr << 1));

    return SUC_OK;
}

FUNCRESULT sx1278SetPreambleLength(SX1278Data *data, INT64 length)
{
    __sx1278WriteRegister(data, REG_PREAMBLE_MSB, (BYTE)(length >> 8));
    __sx1278WriteRegister(data, REG_PREAMBLE_LSB, (BYTE)(length >> 0));

    return SUC_OK;
}

FUNCRESULT sx1278SetSyncWord(SX1278Data *data, INT32 sw)
{
    __sx1278WriteRegister(data, REG_SYNC_WORD, sw);

    return SUC_OK;
}

FUNCRESULT sx1278EnableCrc(SX1278Data *data)
{
    __sx1278WriteRegister(data, REG_MODEM_CONFIG_2, __sx1278ReadRegister(data, REG_MODEM_CONFIG_2) | 0x04);

    return SUC_OK;
}

FUNCRESULT sx1278DisableCrc(SX1278Data *data)
{
    __sx1278WriteRegister(data, REG_MODEM_CONFIG_2, __sx1278ReadRegister(data, REG_MODEM_CONFIG_2) & 0xfb);

    return SUC_OK;
}

FUNCRESULT sx1278EnableInvertIQ(SX1278Data *data)
{
    __sx1278WriteRegister(data, REG_INVERTIQ, 0x66);
    __sx1278WriteRegister(data, REG_INVERTIQ2, 0x19);

    return SUC_OK;
}

FUNCRESULT sx1278DisableInvertIQ(SX1278Data *data)
{
    __sx1278WriteRegister(data, REG_INVERTIQ, 0x27);
    __sx1278WriteRegister(data, REG_INVERTIQ2, 0x1d);

    return SUC_OK;
}

FUNCRESULT sx1278SetOCP(SX1278Data *data, BYTE mA)
{
    BYTE ocpTrim = 27;

    if (mA <= 120)
    {
        ocpTrim = (mA - 45) / 5;
    }
    else if (mA <= 240)
    {
        ocpTrim = (mA + 30) / 10;
    }

    __sx1278WriteRegister(data, REG_OCP, 0x20 | (0x1F & ocpTrim));
}

FUNCRESULT sx1278SetGain(SX1278Data *data, BYTE gain)
{
    if (gain > 6)
    {
        gain = 6;
    }

    sx1278Idle(data);

    if (gain == 0)
    {
        __sx1278WriteRegister(data, REG_MODEM_CONFIG_3, 0x04);
    }
    else
    {
        __sx1278WriteRegister(data, REG_MODEM_CONFIG_3, 0x00);

        __sx1278WriteRegister(data, REG_LNA, 0x03);

        __sx1278WriteRegister(data, REG_LNA, __sx1278ReadRegister(data, REG_LNA) | (gain << 5));
    }

    return SUC_OK;
}

VOID __sx1278ExplicitHeaderMode(SX1278Data *data)
{
    data->_implicitHeaderMode = 0;

    __sx1278WriteRegister(data, REG_MODEM_CONFIG_1, __sx1278ReadRegister(data, REG_MODEM_CONFIG_1) & 0xfe);
}

VOID __sx1278ImplicitHeaderMode(SX1278Data *data)
{
    data->_implicitHeaderMode = 1;

    __sx1278WriteRegister(data, REG_MODEM_CONFIG_1, __sx1278ReadRegister(data, REG_MODEM_CONFIG_1) | 0x01);
}

BOOL __sx1278IsTransmitting(SX1278Data *data)
{
    if ((__sx1278ReadRegister(data, REG_OP_MODE) & MODE_TX) == MODE_TX)
    {
        return TRUE;
    }

    if (__sx1278ReadRegister(data, REG_IRQ_FLAGS) & IRQ_TX_DONE_MASK)
    {
        __sx1278WriteRegister(data, REG_IRQ_FLAGS, IRQ_TX_DONE_MASK);
    }

    return FALSE;
}

INT32 __sx1278GetSpreadingFactor(SX1278Data *data)
{
    return __sx1278ReadRegister(data, REG_MODEM_CONFIG_2) >> 4;
}

INT64 __sx1278GetSignalBandwidth(SX1278Data *data)
{
    BYTE bw = (__sx1278ReadRegister(data, REG_MODEM_CONFIG_1) >> 4);

    switch (bw)
    {
    case 0:
        return 7.8E3;
    case 1:
        return 10.4E3;
    case 2:
        return 15.6E3;
    case 3:
        return 20.8E3;
    case 4:
        return 31.25E3;
    case 5:
        return 41.7E3;
    case 6:
        return 62.5E3;
    case 7:
        return 125E3;
    case 8:
        return 250E3;
    case 9:
        return 500E3;
    }

    return -1;
}

VOID __sx1278SetLdoFlag(SX1278Data *data)
{
    INT64 symbolDuration = 1000 / (__sx1278GetSignalBandwidth(data) / (1L << __sx1278GetSpreadingFactor(data)));
    BOOL ldoOn = symbolDuration > 16;
    BYTE config3 = __sx1278ReadRegister(data, REG_MODEM_CONFIG_3);

    config3 = ldoOn ? config3 | (1 << 3) : config3 & ~(1 << 3);

    __sx1278WriteRegister(data, REG_MODEM_CONFIG_3, config3);
}

BYTE __sx1278ReadRegister(SX1278Data *data, BYTE address)
{
    return __sx1278SingleTransfer(data, address & 0x7f, 0x00);
}

VOID __sx1278WriteRegister(SX1278Data *data, BYTE address, BYTE value)
{
    __sx1278SingleTransfer(data, address | 0x80, value);
}

BYTE __sx1278SingleTransfer(SX1278Data *data, BYTE address, BYTE value)
{
    BYTE response;

    gpioSetPinState(data->_pinout.ss, GPIO_LOW);

    spiWriteBlocking(data->_pinout.spi, &address, 1);
    spiWriteReadBlocking(data->_pinout.spi, &value, &response, 1);

    gpioSetPinState(data->_pinout.ss, GPIO_HIGH);

    return response;
}