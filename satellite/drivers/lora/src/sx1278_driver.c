#include "drivers/lora/sx1278_driver.h"
#include <string.h>
#include <stddef.h>

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

#define IRQ_TX_DONE_MASK 0x08
#define IRQ_PAYLOAD_CRC_ERROR_MASK 0x20
#define IRQ_RX_DONE_MASK 0x40

#define RF_MID_BAND_THRESHOLD 525E6
#define RSSI_OFFSET_HF_PORT 157
#define RSSI_OFFSET_LF_PORT 164

#define MAX_PKT_LENGTH 255

static SX1278Data *s_LoraDatas[LORA_MAX_INSTANCES];
static size_t s_LoraDatasCount = 0;

void loraInit(SX1278Data *data, SX1278Pinout *pinout)
{
    data->_pinout = *pinout;
    data->_txPower = LORA_DEFAULT_TX_POWER;
    data->_frequency = 0;
    data->_packetIndex = 0;
    data->_implicitHeaderMode = 0;
    data->_onReceive = NULL;
    data->_onTxDone = NULL;

    s_LoraDatas[s_LoraDatasCount] = data;
    s_LoraDatasCount++;
}

int loraBegin(SX1278Data *data, long frequency)
{
    gpio_init(data->_pinout.ss);
    gpio_set_dir(data->_pinout.ss, GPIO_OUT);
    gpio_put(data->_pinout.ss, 1);

    if (data->_pinout.reset != -1)
    {
        gpio_init(data->_pinout.reset);
        gpio_set_dir(data->_pinout.reset, GPIO_OUT);

        gpio_put(data->_pinout.reset, 0);
        sleep_ms(10);
        gpio_put(data->_pinout.reset, 1);
        sleep_ms(10);
    }

    spi_init(data->_pinout.spi == 0 ? spi0 : spi1, 12500);
    gpio_set_function(data->_pinout.miso, GPIO_FUNC_SPI);
    gpio_set_function(data->_pinout.sck, GPIO_FUNC_SPI);
    gpio_set_function(data->_pinout.mosi, GPIO_FUNC_SPI);

    gpio_init(data->_pinout.cs);
    gpio_set_dir(data->_pinout.cs, GPIO_OUT);
    gpio_put(data->_pinout.cs, 1);

    uint8_t version = __loraReadRegister(data, REG_VERSION);

    if (version != 0x12)
    {
        return 0;
    }

    loraSleep(data);

    loraSetFrequency(data, frequency);

    __loraWriteRegister(data, REG_FIFO_TX_BASE_ADDR, 0);
    __loraWriteRegister(data, REG_FIFO_RX_BASE_ADDR, 0);
    __loraWriteRegister(data, REG_LNA, __loraReadRegister(data, REG_LNA) | 0x03);
    __loraWriteRegister(data, REG_MODEM_CONFIG_3, 0x04);

    loraSetTxPower(data, data->_txPower);

    loraIdle(data);

    return 1;
}

void loraEnd(SX1278Data *data)
{
    loraSleep(data);

    spi_deinit(data->_pinout.spi == 0 ? spi0 : spi1);
}

int loraBeginPacket(SX1278Data *data, int implicitHeader)
{
    if (__loraIsTransmitting(data))
    {
        return 0;
    }

    loraIdle(data);

    if (implicitHeader)
    {
        __loraImplicitHeaderMode(data);
    }
    else
    {
        __loraExplicitHeaderMode(data);
    }

    __loraWriteRegister(data, REG_FIFO_ADDR_PTR, 0);
    __loraWriteRegister(data, REG_PAYLOAD_LENGTH, 0);

    return 1;
}

int loraEndPacket(SX1278Data *data, bool async)
{
    if ((async) && (data->_onTxDone))
        __loraWriteRegister(data, REG_DIO_MAPPING_1, 0x40);

    __loraWriteRegister(data, REG_OP_MODE, MODE_LONG_RANGE_MODE | MODE_TX);

    if (!async)
    {
        while ((__loraReadRegister(data, REG_IRQ_FLAGS) & IRQ_TX_DONE_MASK) == 0)
        {
            sleep_ms(0);
        }

        __loraWriteRegister(data, REG_IRQ_FLAGS, IRQ_TX_DONE_MASK);
    }

    return 1;
}

int loraParsePacket(SX1278Data *data, int size)
{
    int packetLength = 0;
    int irqFlags = __loraReadRegister(data, REG_IRQ_FLAGS);

    if (size > 0)
    {
        __loraImplicitHeaderMode(data);
        __loraWriteRegister(data, REG_PAYLOAD_LENGTH, size & 0xff);
    }
    else
    {
        __loraExplicitHeaderMode(data);
    }

    __loraWriteRegister(data, REG_IRQ_FLAGS, irqFlags);
    __loraWriteRegister(data, REG_IRQ_FLAGS, irqFlags);

    if ((irqFlags & IRQ_RX_DONE_MASK) && (irqFlags & IRQ_PAYLOAD_CRC_ERROR_MASK) == 0)
    {
        data->_packetIndex = 0;

        if (data->_implicitHeaderMode)
        {
            packetLength = __loraReadRegister(data, REG_PAYLOAD_LENGTH);
        }
        else
        {
            packetLength = __loraReadRegister(data, REG_RX_NB_BYTES);
        }

        __loraWriteRegister(data, REG_FIFO_ADDR_PTR, __loraReadRegister(data, REG_FIFO_RX_CURRENT_ADDR));

        loraIdle(data);
    }
    else if (__loraReadRegister(data, REG_OP_MODE) != (MODE_LONG_RANGE_MODE | MODE_RX_SINGLE))
    {
        __loraWriteRegister(data, REG_FIFO_ADDR_PTR, 0);

        __loraWriteRegister(data, REG_OP_MODE, MODE_LONG_RANGE_MODE | MODE_RX_SINGLE);
    }

    return packetLength;
}

int loraPacketRssi(SX1278Data *data)
{
    return (__loraReadRegister(data, REG_PKT_RSSI_VALUE) - (data->_frequency < RF_MID_BAND_THRESHOLD ? RSSI_OFFSET_LF_PORT : RSSI_OFFSET_HF_PORT));
}

float loraPacketSnr(SX1278Data *data)
{
    return ((int8_t)__loraReadRegister(data, REG_PKT_SNR_VALUE)) * 0.25;
}

long loraPacketFrequencyError(SX1278Data *data)
{
    int32_t freqError = 0;
    freqError = (int32_t)(__loraReadRegister(data, REG_FREQ_ERROR_MSB) & 0x111);
    freqError <<= 8L;
    freqError += (int32_t)(__loraReadRegister(data, REG_FREQ_ERROR_MID));
    freqError <<= 8L;
    freqError += (int32_t)(__loraReadRegister(data, REG_FREQ_ERROR_LSB));

    if (__loraReadRegister(data, REG_FREQ_ERROR_MSB) & 0x1000)
    {
        freqError -= 524288;
    }

    const float fXtal = 32E6;
    const float fError = (((float)(freqError) * (1L << 24)) / fXtal) * (__loraGetSignalBandwidth(data) / 500000.0f);

    return (long)(fError);
}

int loraRssi(SX1278Data *data)
{
    return (__loraReadRegister(data, REG_RSSI_VALUE) - (data->_frequency < RF_MID_BAND_THRESHOLD ? RSSI_OFFSET_LF_PORT : RSSI_OFFSET_HF_PORT));
}

size_t loraWrite(SX1278Data *data, uint8_t byte)
{
    return loraWrite_s(data, &byte, sizeof(byte));
}

size_t loraWrite_s(SX1278Data *data, const uint8_t *buffer, size_t size)
{
    int currentLength = __loraReadRegister(data, REG_PAYLOAD_LENGTH);

    if ((currentLength + size) > MAX_PKT_LENGTH)
    {
        size = MAX_PKT_LENGTH - currentLength;
    }

    for (size_t i = 0; i < size; i++)
    {
        __loraWriteRegister(data, REG_FIFO, buffer[i]);
    }

    __loraWriteRegister(data, REG_PAYLOAD_LENGTH, currentLength + size);

    return size;
}

size_t loraWrite_str(SX1278Data *data, const char *str)
{
    if (str == NULL)
        return 0;

    return loraWrite_s(data, (const uint8_t *)str, strlen(str));
}

size_t loraWrite_str_s(SX1278Data *data, const char *buffer, size_t size)
{
    return loraWrite_s(data, (const uint8_t *)buffer, size);
}

int loraAvailable(SX1278Data *data)
{
    return (__loraReadRegister(data, REG_RX_NB_BYTES) - data->_packetIndex);
}

int loraRead(SX1278Data *data)
{
    if (!loraAvailable(data))
    {
        return -1;
    }

    data->_packetIndex++;

    return __loraReadRegister(data, REG_FIFO);
}

int loraPeek(SX1278Data *data)
{
    if (!loraAvailable(data))
    {
        return -1;
    }

    int currentAddress = __loraReadRegister(data, REG_FIFO_ADDR_PTR);

    uint8_t b = __loraReadRegister(data, REG_FIFO);

    __loraWriteRegister(data, REG_FIFO_ADDR_PTR, currentAddress);

    return b;
}

void loraFlush(SX1278Data *data)
{
}

void loraOnReceive(SX1278Data *data, void (*callback)(int))
{
    data->_onReceive = callback;

    if (callback)
    {
        gpio_set_irq_enabled_with_callback(data->_pinout.dio0, GPIO_IRQ_EDGE_RISE, true, &__loraOnDio0Rise);
    }
    else
    {
        gpio_set_irq_enabled(data->_pinout.dio0, GPIO_IRQ_EDGE_RISE, false);
    }
}

void loraOnTxDone(SX1278Data *data, void (*callback)())
{
    data->_onTxDone = callback;

    if (callback)
    {
        gpio_set_irq_enabled_with_callback(data->_pinout.dio0, GPIO_IRQ_EDGE_RISE, true, &__loraOnDio0Rise);
    }
    else
    {
        gpio_set_irq_enabled(data->_pinout.dio0, GPIO_IRQ_EDGE_RISE, false);
    }
}

void loraReceive(SX1278Data *data, int size)
{
    __loraWriteRegister(data, REG_DIO_MAPPING_1, 0x00);

    if (size > 0)
    {
        __loraImplicitHeaderMode(data);

        __loraWriteRegister(data, REG_PAYLOAD_LENGTH, size & 0xff);
    }
    else
    {
        __loraExplicitHeaderMode(data);
    }

    __loraWriteRegister(data, REG_OP_MODE, MODE_LONG_RANGE_MODE | MODE_RX_CONTINUOUS);
}

void loraIdle(SX1278Data *data)
{
    __loraWriteRegister(data, REG_OP_MODE, MODE_LONG_RANGE_MODE | MODE_STDBY);
}

void loraSleep(SX1278Data *data)
{
    __loraWriteRegister(data, REG_OP_MODE, MODE_LONG_RANGE_MODE | MODE_SLEEP);
}

void loraSetTxPower(SX1278Data *data, int level)
{
    int outputPin = PA_OUTPUT_RFO_PIN;

    if (PA_OUTPUT_RFO_PIN == outputPin)
    {
        if (level < 0)
        {
            level = 0;
        }
        else if (level > 14)
        {
            level = 14;
        }

        __loraWriteRegister(data, REG_PA_CONFIG, 0x70 | level);
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

            __loraWriteRegister(data, REG_PA_DAC, 0x87);
            loraSetOCP(data, 140);
        }
        else
        {
            if (level < 2)
            {
                level = 2;
            }

            __loraWriteRegister(data, REG_PA_DAC, 0x84);
            loraSetOCP(data, 100);
        }

        __loraWriteRegister(data, REG_PA_CONFIG, PA_BOOST | (level - 2));
    }

    data->_txPower = level;
}

void loraSetFrequency(SX1278Data *data, long frequency)
{
    data->_frequency = frequency;

    uint64_t frf = ((uint64_t)frequency << 19) / 32000000;

    __loraWriteRegister(data, REG_FRF_MSB, (uint8_t)(frf >> 16));
    __loraWriteRegister(data, REG_FRF_MID, (uint8_t)(frf >> 8));
    __loraWriteRegister(data, REG_FRF_LSB, (uint8_t)(frf >> 0));
}

void loraSetSpreadingFactor(SX1278Data *data, int sf)
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
        __loraWriteRegister(data, REG_DETECTION_OPTIMIZE, 0xc5);
        __loraWriteRegister(data, REG_DETECTION_THRESHOLD, 0x0c);
    }
    else
    {
        __loraWriteRegister(data, REG_DETECTION_OPTIMIZE, 0xc3);
        __loraWriteRegister(data, REG_DETECTION_THRESHOLD, 0x0a);
    }

    __loraWriteRegister(data, REG_MODEM_CONFIG_2, (__loraReadRegister(data, REG_MODEM_CONFIG_2) & 0x0f) | ((sf << 4) & 0xf0));
    __loraSetLdoFlag(data);
}

void loraSetSignalBandwidth(SX1278Data *data, long sbw)
{
    int bw;

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

    __loraWriteRegister(data, REG_MODEM_CONFIG_1, (__loraReadRegister(data, REG_MODEM_CONFIG_1) & 0x0f) | (bw << 4));
    __loraSetLdoFlag(data);
}

void loraSetCodingRate4(SX1278Data *data, int denominator)
{
    if (denominator < 5)
    {
        denominator = 5;
    }
    else if (denominator > 8)
    {
        denominator = 8;
    }

    int cr = denominator - 4;

    __loraWriteRegister(data, REG_MODEM_CONFIG_1, (__loraReadRegister(data, REG_MODEM_CONFIG_1) & 0xf1) | (cr << 1));
}

void loraSetPreambleLength(SX1278Data *data, long length)
{
    __loraWriteRegister(data, REG_PREAMBLE_MSB, (uint8_t)(length >> 8));
    __loraWriteRegister(data, REG_PREAMBLE_LSB, (uint8_t)(length >> 0));
}

void loraSetSyncWord(SX1278Data *data, int sw)
{
    __loraWriteRegister(data, REG_SYNC_WORD, sw);
}

void loraEnableCrc(SX1278Data *data)
{
    __loraWriteRegister(data, REG_MODEM_CONFIG_2, __loraReadRegister(data, REG_MODEM_CONFIG_2) | 0x04);
}

void loraDisableCrc(SX1278Data *data)
{
    __loraWriteRegister(data, REG_MODEM_CONFIG_2, __loraReadRegister(data, REG_MODEM_CONFIG_2) & 0xfb);
}

void loraEnableInvertIQ(SX1278Data *data)
{
    __loraWriteRegister(data, REG_INVERTIQ, 0x66);
    __loraWriteRegister(data, REG_INVERTIQ2, 0x19);
}

void loraDisableInvertIQ(SX1278Data *data)
{
    __loraWriteRegister(data, REG_INVERTIQ, 0x27);
    __loraWriteRegister(data, REG_INVERTIQ2, 0x1d);
}

void loraSetOCP(SX1278Data *data, uint8_t mA)
{
    uint8_t ocpTrim = 27;

    if (mA <= 120)
    {
        ocpTrim = (mA - 45) / 5;
    }
    else if (mA <= 240)
    {
        ocpTrim = (mA + 30) / 10;
    }

    __loraWriteRegister(data, REG_OCP, 0x20 | (0x1F & ocpTrim));
}

void loraSetGain(SX1278Data *data, uint8_t gain)
{
    if (gain > 6)
    {
        gain = 6;
    }

    loraIdle(data);

    if (gain == 0)
    {
        __loraWriteRegister(data, REG_MODEM_CONFIG_3, 0x04);
    }
    else
    {
        __loraWriteRegister(data, REG_MODEM_CONFIG_3, 0x00);

        __loraWriteRegister(data, REG_LNA, 0x03);

        __loraWriteRegister(data, REG_LNA, __loraReadRegister(data, REG_LNA) | (gain << 5));
    }
}

uint8_t loraRandom(SX1278Data *data)
{
    return __loraReadRegister(data, REG_RSSI_WIDEBAND);
}

void loraSetSPIFrequency(SX1278Data *data, uint32_t frequency)
{
    spi_set_baudrate(data->_pinout.spi == 0 ? spi0 : spi1, frequency);
}

void loraDumpRegisters(SX1278Data *data, uint8_t buffer[128])
{
    for (int i = 0; i < 128; i++)
    {
        buffer[i] = __loraReadRegister(data, i);
    }
}

void __loraExplicitHeaderMode(SX1278Data *data)
{
    data->_implicitHeaderMode = 0;

    __loraWriteRegister(data, REG_MODEM_CONFIG_1, __loraReadRegister(data, REG_MODEM_CONFIG_1) & 0xfe);
}

void __loraImplicitHeaderMode(SX1278Data *data)
{
    data->_implicitHeaderMode = 1;

    __loraWriteRegister(data, REG_MODEM_CONFIG_1, __loraReadRegister(data, REG_MODEM_CONFIG_1) | 0x01);
}

void __loraHandleDio0Rise(SX1278Data *data)
{
    int irqFlags = __loraReadRegister(data, REG_IRQ_FLAGS);

    __loraWriteRegister(data, REG_IRQ_FLAGS, irqFlags);
    __loraWriteRegister(data, REG_IRQ_FLAGS, irqFlags);

    if ((irqFlags & IRQ_PAYLOAD_CRC_ERROR_MASK) == 0)
    {
        if ((irqFlags & IRQ_RX_DONE_MASK) != 0)
        {
            data->_packetIndex = 0;

            int packetLength = data->_implicitHeaderMode ? __loraReadRegister(data, REG_PAYLOAD_LENGTH) : __loraReadRegister(data, REG_RX_NB_BYTES);

            __loraWriteRegister(data, REG_FIFO_ADDR_PTR, __loraReadRegister(data, REG_FIFO_RX_CURRENT_ADDR));

            if (data->_onReceive)
            {
                data->_onReceive(packetLength);
            }
        }
        else if ((irqFlags & IRQ_TX_DONE_MASK) != 0)
        {
            if (data->_onTxDone)
            {
                data->_onTxDone();
            }
        }
    }
}

bool __loraIsTransmitting(SX1278Data *data)
{
    if ((__loraReadRegister(data, REG_OP_MODE) & MODE_TX) == MODE_TX)
    {
        return true;
    }

    if (__loraReadRegister(data, REG_IRQ_FLAGS) & IRQ_TX_DONE_MASK)
    {
        __loraWriteRegister(data, REG_IRQ_FLAGS, IRQ_TX_DONE_MASK);
    }

    return false;
}

int __loraGetSpreadingFactor(SX1278Data *data)
{
    return __loraReadRegister(data, REG_MODEM_CONFIG_2) >> 4;
}

long __loraGetSignalBandwidth(SX1278Data *data)
{
    uint8_t bw = (__loraReadRegister(data, REG_MODEM_CONFIG_1) >> 4);

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

void __loraSetLdoFlag(SX1278Data *data)
{
    long symbolDuration = 1000 / (__loraGetSignalBandwidth(data) / (1L << __loraGetSpreadingFactor(data)));

    bool ldoOn = symbolDuration > 16;

    uint8_t config3 = __loraReadRegister(data, REG_MODEM_CONFIG_3);

    config3 = ldoOn ? config3 | (1 << 3) : config3 & ~(1 << 3);

    __loraWriteRegister(data, REG_MODEM_CONFIG_3, config3);
}

uint8_t __loraReadRegister(SX1278Data *data, uint8_t address)
{
    return __loraSingleTransfer(data, address & 0x7f, 0x00);
}

void __loraWriteRegister(SX1278Data *data, uint8_t address, uint8_t value)
{
    __loraSingleTransfer(data, address | 0x80, value);
}

uint8_t __loraSingleTransfer(SX1278Data *data, uint8_t address, uint8_t value)
{
    uint8_t response;

    gpio_put(data->_pinout.ss, 0);

    spi_write_blocking(data->_pinout.spi == 0 ? spi0 : spi1, &address, 1);
    spi_write_read_blocking(data->_pinout.spi == 0 ? spi0 : spi1, &value, &response, 1);

    gpio_put(data->_pinout.ss, 1);

    return response;
}

static SX1278Data *__loraDataFromGPIO(uint gpio)
{
    for (size_t i = 0; i < s_LoraDatasCount; i++)
    {
        SX1278Data *d = s_LoraDatas[i];
        LoRaPinout _pinout = d->_pinout;

        if (_pinout.cs == gpio || _pinout.miso == gpio || _pinout.mosi == gpio || _pinout.sck == gpio || _pinout.ss == gpio || _pinout.reset == gpio || _pinout.dio0 == gpio)
        {
            return d;
        }
    }

    return NULL;
}

void __loraOnDio0Rise(uint gpio, uint32_t events)
{
    gpio_acknowledge_irq(gpio, events);

    SX1278Data *data = __loraDataFromGPIO(gpio);

    __loraHandleDio0Rise(data);
}