#include "lora.h"
#include "logger.h"
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

static lora_data_t *s_LoraDatas[LORA_MAX_INSTANCES];
static size_t s_LoraDatasCount = 0;

void loraInit(lora_data_t *data, lora_pinout_t *pinout)
{
    data->_frequency = 0;
    data->_packetIndex = 0;
    data->_implicitHeaderMode = 0;
    data->_onReceive = NULL;
    data->_onTxDone = NULL;

    s_LoraDatas[s_LoraDatasCount] = data;
    s_LoraDatasCount++;
}

int loraBegin(lora_data_t *data, long frequency)
{
    gpio_init(data->pinout.ss);
    gpio_set_dir(data->pinout.ss, GPIO_OUT);
    gpio_put(data->pinout.ss, 1);

    if (data->pinout.reset != -1)
    {
        gpio_init(data->pinout.reset);
        gpio_set_dir(data->pinout.reset, GPIO_OUT);

        gpio_put(data->pinout.reset, 0);
        sleep_ms(10);
        gpio_put(data->pinout.reset, 1);
        sleep_ms(10);
    }

    spi_init(data->pinout.spi == 0 ? spi0 : spi1, 12500);
    gpio_set_function(data->pinout.miso, GPIO_FUNC_SPI);
    gpio_set_function(data->pinout.sck, GPIO_FUNC_SPI);
    gpio_set_function(data->pinout.mosi, GPIO_FUNC_SPI);

    gpio_init(data->pinout.cs);
    gpio_set_dir(data->pinout.cs, GPIO_OUT);
    gpio_put(data->pinout.cs, 1);

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

    loraSetTxPower(data, 17, PA_OUTPUT_PA_BOOST_PIN);

    loraIdle(data);

    return 1;
}

void loraEnd(lora_data_t *data)
{
    loraSleep(data);

    spi_deinit(data->pinout.spi == 0 ? spi0 : spi1);
}

int loraBeginPacket(lora_data_t *data, int implicitHeader)
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

int loraEndPacket(lora_data_t *data, bool async)
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

int loraParsePacket(lora_data_t *data, int size)
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

int loraPacketRssi(lora_data_t *data)
{
    return (__loraReadRegister(data, REG_PKT_RSSI_VALUE) - (data->_frequency < RF_MID_BAND_THRESHOLD ? RSSI_OFFSET_LF_PORT : RSSI_OFFSET_HF_PORT));
}

float loraPacketSnr(lora_data_t *data)
{
    return ((int8_t)__loraReadRegister(data, REG_PKT_SNR_VALUE)) * 0.25;
}

long loraPacketFrequencyError(lora_data_t *data)
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

int loraRssi(lora_data_t *data)
{
    return (__loraReadRegister(data, REG_RSSI_VALUE) - (data->_frequency < RF_MID_BAND_THRESHOLD ? RSSI_OFFSET_LF_PORT : RSSI_OFFSET_HF_PORT));
}

size_t loraWrite(lora_data_t *data, uint8_t byte)
{
    return loraWrite_s(data, &byte, sizeof(byte));
}

size_t loraWrite_s(lora_data_t *data, const uint8_t *buffer, size_t size)
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

size_t loraWrite_str(lora_data_t *data, const char *str)
{
    if (str == NULL)
        return 0;

    return loraWrite_s(data, (const uint8_t *)str, strlen(str));
}

size_t loraWrite_str_s(lora_data_t *data, const char *buffer, size_t size)
{
    return loraWrite_s(data, (const uint8_t *)buffer, size);
}

int loraAvailable(lora_data_t *data)
{
    return (__loraReadRegister(data, REG_RX_NB_BYTES) - data->_packetIndex);
}

int loraRead(lora_data_t *data)
{
    if (!loraAvailable(data))
    {
        return -1;
    }

    data->_packetIndex++;

    return __loraReadRegister(data, REG_FIFO);
}

int loraPeek(lora_data_t *data)
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

void loraFlush(lora_data_t *data)
{
}

void loraOnReceive(lora_data_t *data, void (*callback)(int))
{
    data->_onReceive = callback;

    if (callback)
    {
        gpio_set_irq_enabled_with_callback(data->pinout.dio0, GPIO_IRQ_EDGE_RISE, true, &__loraOnDio0Rise);
    }
    else
    {
        gpio_set_irq_enabled(data->pinout.dio0, GPIO_IRQ_EDGE_RISE, false);
    }
}

void loraOnTxDone(lora_data_t *data, void (*callback)())
{
    data->_onTxDone = callback;

    if (callback)
    {
        gpio_set_irq_enabled_with_callback(data->pinout.dio0, GPIO_IRQ_EDGE_RISE, true, &__loraOnDio0Rise);
    }
    else
    {
        gpio_set_irq_enabled(data->pinout.dio0, GPIO_IRQ_EDGE_RISE, false);
    }
}

void loraReceive(lora_data_t *data, int size)
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

void loraIdle(lora_data_t *data)
{
    __loraWriteRegister(data, REG_OP_MODE, MODE_LONG_RANGE_MODE | MODE_STDBY);
}

void loraSleep(lora_data_t *data)
{
    __loraWriteRegister(data, REG_OP_MODE, MODE_LONG_RANGE_MODE | MODE_SLEEP);
}

void loraSetTxPower(lora_data_t *data, int level, int outputPin)
{
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
}

void loraSetFrequency(lora_data_t *data, long frequency)
{
    data->_frequency = frequency;

    uint64_t frf = ((uint64_t)frequency << 19) / 32000000;

    __loraWriteRegister(data, REG_FRF_MSB, (uint8_t)(frf >> 16));
    __loraWriteRegister(data, REG_FRF_MID, (uint8_t)(frf >> 8));
    __loraWriteRegister(data, REG_FRF_LSB, (uint8_t)(frf >> 0));
}

void loraSetSpreadingFactor(lora_data_t *data, int sf)
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

void setSignalBandwidth(lora_data_t *data, long sbw)
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

void loraSetCodingRate4(lora_data_t *data, int denominator)
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

void loraSetPreambleLength(lora_data_t *data, long length)
{
    __loraWriteRegister(data, REG_PREAMBLE_MSB, (uint8_t)(length >> 8));
    __loraWriteRegister(data, REG_PREAMBLE_LSB, (uint8_t)(length >> 0));
}

void loraSetSyncWord(lora_data_t *data, int sw)
{
    __loraWriteRegister(data, REG_SYNC_WORD, sw);
}

void loraEnableCrc(lora_data_t *data)
{
    __loraWriteRegister(data, REG_MODEM_CONFIG_2, __loraReadRegister(data, REG_MODEM_CONFIG_2) | 0x04);
}

void loraDisableCrc(lora_data_t *data)
{
    __loraWriteRegister(data, REG_MODEM_CONFIG_2, __loraReadRegister(data, REG_MODEM_CONFIG_2) & 0xfb);
}

void loraEnableInvertIQ(lora_data_t *data)
{
    __loraWriteRegister(data, REG_INVERTIQ, 0x66);
    __loraWriteRegister(data, REG_INVERTIQ2, 0x19);
}

void loraDisableInvertIQ(lora_data_t *data)
{
    __loraWriteRegister(data, REG_INVERTIQ, 0x27);
    __loraWriteRegister(data, REG_INVERTIQ2, 0x1d);
}

void loraSetOCP(lora_data_t *data, uint8_t mA)
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

void loraSetGain(lora_data_t *data, uint8_t gain)
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

uint8_t loraRandom(lora_data_t *data)
{
    return __loraReadRegister(data, REG_RSSI_WIDEBAND);
}

void loraSetSPIFrequency(lora_data_t *data, uint32_t frequency)
{
    spi_set_baudrate(data->pinout.spi == 0 ? spi0 : spi1, frequency);
}

void loraDumpRegisters(lora_data_t *data)
{
    for (int i = 0; i < 128; i++)
    {
        MY_LOG_CORE_INFO("0x%x: 0x%x", i, __loraReadRegister(data, i));
    }
}

void __loraExplicitHeaderMode(lora_data_t *data)
{
    data->_implicitHeaderMode = 0;

    __loraWriteRegister(data, REG_MODEM_CONFIG_1, __loraReadRegister(data, REG_MODEM_CONFIG_1) & 0xfe);
}

void __loraImplicitHeaderMode(lora_data_t *data)
{
    data->_implicitHeaderMode = 1;

    __loraWriteRegister(data, REG_MODEM_CONFIG_1, __loraReadRegister(data, REG_MODEM_CONFIG_1) | 0x01);
}

void __loraHandleDio0Rise(lora_data_t *data)
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

bool __loraIsTransmitting(lora_data_t *data)
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

int loraGetSpreadingFactor(lora_data_t *data)
{
    return __loraReadRegister(data, REG_MODEM_CONFIG_2) >> 4;
}

long __loraGetSignalBandwidth(lora_data_t *data)
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

void __loraSetLdoFlag(lora_data_t *data)
{
    long symbolDuration = 1000 / (__loraGetSignalBandwidth(data) / (1L << __loraGetSpreadingFactor(data)));

    bool ldoOn = symbolDuration > 16;

    uint8_t config3 = __loraReadRegister(data, REG_MODEM_CONFIG_3);

    config3 = ldoOn ? config3 | (1 << 3) : config3 & ~(1 << 3);

    __loraWriteRegister(data, REG_MODEM_CONFIG_3, config3);
}

uint8_t __loraReadRegister(lora_data_t *data, uint8_t address)
{
    return __loraSingleTransfer(data, address & 0x7f, 0x00);
}

void __loraWriteRegister(lora_data_t *data, uint8_t address, uint8_t value)
{
    __loraSingleTransfer(data, address | 0x80, value);
}

uint8_t __loraSingleTransfer(lora_data_t *data, uint8_t address, uint8_t value)
{
    uint8_t response;

    gpio_put(data->pinout.ss, 0);

    spi_write_blocking(data->pinout.spi == 0 ? spi0 : spi1, &address, 1);
    spi_write_read_blocking(data->pinout.spi == 0 ? spi0 : spi1, &value, &response, 1);

    gpio_put(data->pinout.ss, 1);

    return response;
}

static lora_data_t *__loraDataFromGPIO(uint gpio)
{
    for (size_t i = 0; i < s_LoraDatasCount; i++)
    {
        lora_data_t *d = s_LoraDatas[i];
        lora_pinout_t pinout = d->pinout;

        if (pinout.cs == gpio || pinout.miso == gpio || pinout.mosi == gpio || pinout.sck == gpio || pinout.ss == gpio || pinout.reset == gpio || pinout.dio0 == gpio)
        {
            return d;
        }
    }

    return NULL;
}

void __loraOnDio0Rise(uint gpio, uint32_t events)
{
    gpio_acknowledge_irq(gpio, events);

    lora_data_t *data = __loraDataFromGPIO(gpio);

    __loraHandleDio0Rise(data);
}