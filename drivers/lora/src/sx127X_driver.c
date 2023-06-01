#include "drivers/lora/sx127X_driver.h"
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

FUNCRESULT sx127XInit(SX127XData *data, SX127XPinout *pinout, UINT64 frequency)
{
    if (!data || !pinout)
    {
        return ERR_INVALIDARG;
    }

    data->pinout = *pinout;
    data->txPower = LORA_DEFAULT_TX_POWER;
    data->frequency = frequency;
    data->packetIndex = 0;
    data->implicitHeaderMode = FALSE;

    gpioInitPin(data->pinout.cs, GPIO_OUTPUT);
    gpioSetPinState(data->pinout.cs, GPIO_HIGH);

    if (data->pinout.reset != -1)
    {
        gpioInitPin(data->pinout.reset, GPIO_OUTPUT);

        gpioSetPinState(data->pinout.reset, GPIO_LOW);
        sleep_ms(10);

        gpioSetPinState(data->pinout.reset, GPIO_HIGH);
        sleep_ms(10);
    }

    spiInitPins(data->pinout.spi, data->pinout.miso, data->pinout.mosi, data->pinout.sck, data->pinout.cs);

    BYTE version = __sx127XReadRegister(data, REG_VERSION);

    if (version != 0x12)
    {
        return ERR_UNEXPECTED;
    }

    sx127XSleep(data);

    sx127XSetFrequency(data, data->frequency);
    sx127XSetTxPower(data, data->txPower);

    __sx127XWriteRegister(data, REG_FIFO_TX_BASE_ADDR, 0);
    __sx127XWriteRegister(data, REG_FIFO_RX_BASE_ADDR, 0);
    __sx127XWriteRegister(data, REG_LNA, __sx127XReadRegister(data, REG_LNA) | 0x03);
    __sx127XWriteRegister(data, REG_MODEM_CONFIG_3, 0x04);

    sx127XIdle(data);

    return SUC_OK;
}

FUNCRESULT sx127XWriteBuffer(SX127XData *data, const BYTE *buffer, SIZE size)
{
    if (__sx127XIsTransmitting(data))
    {
        return ERR_ACCESSDENIED;
    }

    sx127XIdle(data);

    __sx127XExplicitHeaderMode(data);
    __sx127XWriteRegister(data, REG_FIFO_ADDR_PTR, 0);
    __sx127XWriteRegister(data, REG_PAYLOAD_LENGTH, 0);

    if (size > MAX_PKT_LENGTH)
    {
        size = MAX_PKT_LENGTH;
    }

    for (SIZE i = 0; i < size; i++)
    {
        __sx127XWriteRegister(data, REG_FIFO, buffer[i]);
    }

    __sx127XWriteRegister(data, REG_PAYLOAD_LENGTH, size);
    __sx127XWriteRegister(data, REG_OP_MODE, MODE_LONG_RANGE_MODE | MODE_TX);

    while ((__sx127XReadRegister(data, REG_IRQ_FLAGS) & IRQ_TX_DONE_MASK) == 0)
    {
        sleep_ms(0);
    }

    __sx127XWriteRegister(data, REG_IRQ_FLAGS, IRQ_TX_DONE_MASK);

    return SUC_OK;
}

FUNCRESULT sx127XAvailable(SX127XData *data, BOOL *available)
{
    *available = (__sx127XReadRegister(data, REG_RX_NB_BYTES) - data->packetIndex);

    return SUC_OK;
}

FUNCRESULT sx127XParsePacket(SX127XData *data, SIZE size, SIZE *packetLengthOut)
{
    SIZE packetLength = 0;
    INT32 irqFlags = __sx127XReadRegister(data, REG_IRQ_FLAGS);

    if (size > 0)
    {
        __sx127XImplicitHeaderMode(data);
        __sx127XWriteRegister(data, REG_PAYLOAD_LENGTH, size & 0xff);
    }
    else
    {
        __sx127XExplicitHeaderMode(data);
    }

    __sx127XWriteRegister(data, REG_IRQ_FLAGS, irqFlags);
    __sx127XWriteRegister(data, REG_IRQ_FLAGS, irqFlags);

    if ((irqFlags & IRQ_RX_DONE_MASK) && (irqFlags & IRQ_PAYLOAD_CRC_ERROR_MASK) == 0)
    {
        data->packetIndex = 0;

        if (data->implicitHeaderMode)
        {
            packetLength = __sx127XReadRegister(data, REG_PAYLOAD_LENGTH);
        }
        else
        {
            packetLength = __sx127XReadRegister(data, REG_RX_NB_BYTES);
        }

        __sx127XWriteRegister(data, REG_FIFO_ADDR_PTR, __sx127XReadRegister(data, REG_FIFO_RX_CURRENT_ADDR));

        sx127XIdle(data);
    }
    else if (__sx127XReadRegister(data, REG_OP_MODE) != (MODE_LONG_RANGE_MODE | MODE_RX_SINGLE))
    {
        __sx127XWriteRegister(data, REG_FIFO_ADDR_PTR, 0);
        __sx127XWriteRegister(data, REG_OP_MODE, MODE_LONG_RANGE_MODE | MODE_RX_SINGLE);
    }

    *packetLengthOut = packetLength;

    return SUC_OK;
}

FUNCRESULT sx127XRead(SX127XData *data, BYTE *dataOut)
{
    BOOL available = FALSE;

    sx127XAvailable(data, &available);

    if (!available)
    {
        return ERR_ACCESSDENIED;
    }

    data->packetIndex++;

    *dataOut = __sx127XReadRegister(data, REG_FIFO);

    return SUC_OK;
}

FUNCRESULT sx127XPeek(SX127XData *data, BYTE *dataOut)
{
    BOOL available = FALSE;

    sx127XAvailable(data, &available);

    if (!available)
    {
        return ERR_ACCESSDENIED;
    }

    BYTE currentAddress = __sx127XReadRegister(data, REG_FIFO_ADDR_PTR);

    *dataOut = __sx127XReadRegister(data, REG_FIFO);

    __sx127XWriteRegister(data, REG_FIFO_ADDR_PTR, currentAddress);

    return SUC_OK;
}

FUNCRESULT sx127XIdle(SX127XData *data)
{
    __sx127XWriteRegister(data, REG_OP_MODE, MODE_LONG_RANGE_MODE | MODE_STDBY);

    return SUC_OK;
}

FUNCRESULT sx127XSleep(SX127XData *data)
{
    __sx127XWriteRegister(data, REG_OP_MODE, MODE_LONG_RANGE_MODE | MODE_SLEEP);

    return SUC_OK;
}

FUNCRESULT sx127XPacketRssi(SX127XData *data, INT32 *rssi)
{
    *rssi = (__sx127XReadRegister(data, REG_PKT_RSSI_VALUE) - (data->frequency < RF_MID_BAND_THRESHOLD ? RSSI_OFFSET_LF_PORT : RSSI_OFFSET_HF_PORT));

    return SUC_OK;
}

FUNCRESULT sx127XPacketSnr(SX127XData *data, FLOAT *snr)
{
    *snr = ((UINT8)__sx127XReadRegister(data, REG_PKT_SNR_VALUE)) * 0.25;

    return SUC_OK;
}

FUNCRESULT sx127XPacketFrequencyError(SX127XData *data, INT64 *error)
{
    INT32 freqError = 0;
    freqError = (INT32)(__sx127XReadRegister(data, REG_FREQ_ERROR_MSB) & 0x111);
    freqError <<= 8L;
    freqError += (INT32)(__sx127XReadRegister(data, REG_FREQ_ERROR_MID));
    freqError <<= 8L;
    freqError += (INT32)(__sx127XReadRegister(data, REG_FREQ_ERROR_LSB));

    if (__sx127XReadRegister(data, REG_FREQ_ERROR_MSB) & 0x1000)
    {
        freqError -= 524288;
    }

    const FLOAT fXtal = 32E6;
    const FLOAT fError = (((FLOAT)(freqError) * (1L << 24)) / fXtal) * (__sx127XGetSignalBandwidth(data) / 500000.0f);

    *error = (long)(fError);

    return SUC_OK;
}

FUNCRESULT sx127XRssi(SX127XData *data, INT32 *rssi)
{
    *rssi = (__sx127XReadRegister(data, REG_RSSI_VALUE) - (data->frequency < RF_MID_BAND_THRESHOLD ? RSSI_OFFSET_LF_PORT : RSSI_OFFSET_HF_PORT));

    return SUC_OK;
}

FUNCRESULT sx127XSetTxPower(SX127XData *data, INT32 level)
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

        __sx127XWriteRegister(data, REG_PA_CONFIG, 0x70 | level);
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

            __sx127XWriteRegister(data, REG_PA_DAC, 0x87);
            sx127XSetOCP(data, 140);
        }
        else
        {
            if (level < 2)
            {
                level = 2;
            }

            __sx127XWriteRegister(data, REG_PA_DAC, 0x84);
            sx127XSetOCP(data, 100);
        }

        __sx127XWriteRegister(data, REG_PA_CONFIG, PA_BOOST | (level - 2));
    }

    data->txPower = level;

    return SUC_OK;
}

FUNCRESULT sx127XSetFrequency(SX127XData *data, UINT64 frequency)
{
    data->frequency = frequency;

    UINT64 frf = (frequency << 19) / 32000000;

    __sx127XWriteRegister(data, REG_FRF_MSB, (BYTE)(frf >> 16));
    __sx127XWriteRegister(data, REG_FRF_MID, (BYTE)(frf >> 8));
    __sx127XWriteRegister(data, REG_FRF_LSB, (BYTE)(frf >> 0));

    return SUC_OK;
}

FUNCRESULT sx127XSetSpreadingFactor(SX127XData *data, INT32 sf)
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
        __sx127XWriteRegister(data, REG_DETECTION_OPTIMIZE, 0xc5);
        __sx127XWriteRegister(data, REG_DETECTION_THRESHOLD, 0x0c);
    }
    else
    {
        __sx127XWriteRegister(data, REG_DETECTION_OPTIMIZE, 0xc3);
        __sx127XWriteRegister(data, REG_DETECTION_THRESHOLD, 0x0a);
    }

    __sx127XWriteRegister(data, REG_MODEM_CONFIG_2, (__sx127XReadRegister(data, REG_MODEM_CONFIG_2) & 0x0f) | ((sf << 4) & 0xf0));
    __sx127XSetLdoFlag(data);

    return SUC_OK;
}

FUNCRESULT sx127XSetSignalBandwidth(SX127XData *data, INT64 sbw)
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

    __sx127XWriteRegister(data, REG_MODEM_CONFIG_1, (__sx127XReadRegister(data, REG_MODEM_CONFIG_1) & 0x0f) | (bw << 4));
    __sx127XSetLdoFlag(data);

    return SUC_OK;
}

FUNCRESULT sx127XSetCodingRate4(SX127XData *data, INT32 denominator)
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

    __sx127XWriteRegister(data, REG_MODEM_CONFIG_1, (__sx127XReadRegister(data, REG_MODEM_CONFIG_1) & 0xf1) | (cr << 1));

    return SUC_OK;
}

FUNCRESULT sx127XSetPreambleLength(SX127XData *data, INT64 length)
{
    __sx127XWriteRegister(data, REG_PREAMBLE_MSB, (BYTE)(length >> 8));
    __sx127XWriteRegister(data, REG_PREAMBLE_LSB, (BYTE)(length >> 0));

    return SUC_OK;
}

FUNCRESULT sx127XSetSyncWord(SX127XData *data, INT32 sw)
{
    __sx127XWriteRegister(data, REG_SYNC_WORD, sw);

    return SUC_OK;
}

FUNCRESULT sx127XEnableCrc(SX127XData *data)
{
    __sx127XWriteRegister(data, REG_MODEM_CONFIG_2, __sx127XReadRegister(data, REG_MODEM_CONFIG_2) | 0x04);

    return SUC_OK;
}

FUNCRESULT sx127XDisableCrc(SX127XData *data)
{
    __sx127XWriteRegister(data, REG_MODEM_CONFIG_2, __sx127XReadRegister(data, REG_MODEM_CONFIG_2) & 0xfb);

    return SUC_OK;
}

FUNCRESULT sx127XEnableInvertIQ(SX127XData *data)
{
    __sx127XWriteRegister(data, REG_INVERTIQ, 0x66);
    __sx127XWriteRegister(data, REG_INVERTIQ2, 0x19);

    return SUC_OK;
}

FUNCRESULT sx127XDisableInvertIQ(SX127XData *data)
{
    __sx127XWriteRegister(data, REG_INVERTIQ, 0x27);
    __sx127XWriteRegister(data, REG_INVERTIQ2, 0x1d);

    return SUC_OK;
}

FUNCRESULT sx127XSetOCP(SX127XData *data, BYTE mA)
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

    __sx127XWriteRegister(data, REG_OCP, 0x20 | (0x1F & ocpTrim));
}

FUNCRESULT sx127XSetGain(SX127XData *data, BYTE gain)
{
    if (gain > 6)
    {
        gain = 6;
    }

    sx127XIdle(data);

    if (gain == 0)
    {
        __sx127XWriteRegister(data, REG_MODEM_CONFIG_3, 0x04);
    }
    else
    {
        __sx127XWriteRegister(data, REG_MODEM_CONFIG_3, 0x00);

        __sx127XWriteRegister(data, REG_LNA, 0x03);

        __sx127XWriteRegister(data, REG_LNA, __sx127XReadRegister(data, REG_LNA) | (gain << 5));
    }

    return SUC_OK;
}

VOID __sx127XExplicitHeaderMode(SX127XData *data)
{
    data->implicitHeaderMode = FALSE;

    __sx127XWriteRegister(data, REG_MODEM_CONFIG_1, __sx127XReadRegister(data, REG_MODEM_CONFIG_1) & 0xfe);
}

VOID __sx127XImplicitHeaderMode(SX127XData *data)
{
    data->implicitHeaderMode = TRUE;

    __sx127XWriteRegister(data, REG_MODEM_CONFIG_1, __sx127XReadRegister(data, REG_MODEM_CONFIG_1) | 0x01);
}

BOOL __sx127XIsTransmitting(SX127XData *data)
{
    if ((__sx127XReadRegister(data, REG_OP_MODE) & MODE_TX) == MODE_TX)
    {
        return TRUE;
    }

    if (__sx127XReadRegister(data, REG_IRQ_FLAGS) & IRQ_TX_DONE_MASK)
    {
        __sx127XWriteRegister(data, REG_IRQ_FLAGS, IRQ_TX_DONE_MASK);
    }

    return FALSE;
}

INT32 __sx127XGetSpreadingFactor(SX127XData *data)
{
    return __sx127XReadRegister(data, REG_MODEM_CONFIG_2) >> 4;
}

INT64 __sx127XGetSignalBandwidth(SX127XData *data)
{
    BYTE bw = (__sx127XReadRegister(data, REG_MODEM_CONFIG_1) >> 4);

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

VOID __sx127XSetLdoFlag(SX127XData *data)
{
    INT64 symbolDuration = 1000 / (__sx127XGetSignalBandwidth(data) / (1L << __sx127XGetSpreadingFactor(data)));
    BOOL ldoOn = symbolDuration > 16;
    BYTE config3 = __sx127XReadRegister(data, REG_MODEM_CONFIG_3);

    config3 = ldoOn ? config3 | (1 << 3) : config3 & ~(1 << 3);

    __sx127XWriteRegister(data, REG_MODEM_CONFIG_3, config3);
}

BYTE __sx127XReadRegister(SX127XData *data, BYTE address)
{
    return __sx127XSingleTransfer(data, address & 0x7f, 0x00);
}

VOID __sx127XWriteRegister(SX127XData *data, BYTE address, BYTE value)
{
    __sx127XSingleTransfer(data, address | 0x80, value);
}

BYTE __sx127XSingleTransfer(SX127XData *data, BYTE address, BYTE value)
{
    BYTE response;

    gpioSetPinState(data->pinout.cs, GPIO_LOW);

    spiWriteBlocking(data->pinout.spi, &address, 1);
    spiWriteReadBlocking(data->pinout.spi, &value, &response, 1);

    gpioSetPinState(data->pinout.cs, GPIO_HIGH);

    return response;
}