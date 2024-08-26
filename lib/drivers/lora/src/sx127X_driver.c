#include "lib/drivers/lora/sx127X_driver.h"
#include "hal/time_tracker.h"
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
#define REG_RX_NB_uint8_tS 0x13
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

static void _sx127x_explicit_header_mode(sx127x_config_t *config);
static void _sx127x_implicit_header_mode(sx127x_config_t *config);
static bool _sx127x_is_transmitting(const sx127x_config_t *config);
static int _sx127X_get_spreading_factor(const sx127x_config_t *config);
static long _sx127XGetSignalBandwidth(const sx127x_config_t *config);
static void _sx127x_set_ldo_flag(const sx127x_config_t *config);
static uint8_t _sx127x_read_register(const sx127x_config_t *config, uint8_t address);
static void _sx127x_write_register(const sx127x_config_t *config, uint8_t address, uint8_t value);
static uint8_t _sx127x_single_transfer(const sx127x_config_t *config, uint8_t address, uint8_t value);

void sx127x_init(sx127x_config_t *config, hal_spi_instance_t spi, hal_pin_number_t cs, hal_pin_number_t reset, unsigned long long frequency)
{
    config->spi = spi;
    config->cs = cs;
    config->reset = reset;
    config->packetIndex = 0;
    config->implicitHeaderMode = false;

    hal_gpio_init_pin(cs, GPIO_OUTPUT);
    hal_gpio_set_pin_state(cs, GPIO_HIGH);

    if (reset != -1)
    {
        hal_gpio_init_pin(reset, GPIO_OUTPUT);

        hal_gpio_set_pin_state(reset, GPIO_LOW);
        hal_time_sleep_ms(10);

        hal_gpio_set_pin_state(reset, GPIO_HIGH);
        hal_time_sleep_ms(10);
    }

    hal_spi_init_cs(cs);

    uint8_t version = _sx127x_read_register(config, REG_VERSION);

    if (version != 0x12)
    {
        return;
    }

    sx127x_sleep(config);

    sx127x_set_frequency(config, frequency);
    sx127x_set_tx_power(config, LORA_DEFAULT_TX_POWER);

    _sx127x_write_register(config, REG_FIFO_TX_BASE_ADDR, 0);
    _sx127x_write_register(config, REG_FIFO_RX_BASE_ADDR, 0);
    _sx127x_write_register(config, REG_LNA, _sx127x_read_register(config, REG_LNA) | 0x03);
    _sx127x_write_register(config, REG_MODEM_CONFIG_3, 0x04);

    sx127x_idle(config);
}

void sx127x_write_buffer(sx127x_config_t *config, const uint8_t *buffer, size_t size)
{
    if (_sx127x_is_transmitting(config))
    {
        return;
    }

    sx127x_idle(config);

    _sx127x_explicit_header_mode(config);
    _sx127x_write_register(config, REG_FIFO_ADDR_PTR, 0);
    _sx127x_write_register(config, REG_PAYLOAD_LENGTH, 0);

    if (size > MAX_PKT_LENGTH)
    {
        size = MAX_PKT_LENGTH;
    }

    for (size_t i = 0; i < size; i++)
    {
        _sx127x_write_register(config, REG_FIFO, buffer[i]);
    }

    _sx127x_write_register(config, REG_PAYLOAD_LENGTH, size);
    _sx127x_write_register(config, REG_OP_MODE, MODE_LONG_RANGE_MODE | MODE_TX);

    while ((_sx127x_read_register(config, REG_IRQ_FLAGS) & IRQ_TX_DONE_MASK) == 0)
    {
        hal_time_sleep_ms(0);
    }

    _sx127x_write_register(config, REG_IRQ_FLAGS, IRQ_TX_DONE_MASK);
}

bool sx127x_available(const sx127x_config_t *config)
{
    return _sx127x_read_register(config, REG_RX_NB_uint8_tS) - config->packetIndex;
}

size_t sx127x_parse_packet(sx127x_config_t *config, size_t size)
{
    size_t packetLength = 0;
    int irqFlags = _sx127x_read_register(config, REG_IRQ_FLAGS);

    if (size > 0)
    {
        _sx127x_implicit_header_mode(config);
        _sx127x_write_register(config, REG_PAYLOAD_LENGTH, size & 0xff);
    }
    else
    {
        _sx127x_explicit_header_mode(config);
    }

    _sx127x_write_register(config, REG_IRQ_FLAGS, irqFlags);
    _sx127x_write_register(config, REG_IRQ_FLAGS, irqFlags);

    if ((irqFlags & IRQ_RX_DONE_MASK) && (irqFlags & IRQ_PAYLOAD_CRC_ERROR_MASK) == 0)
    {
        config->packetIndex = 0;

        if (config->implicitHeaderMode)
        {
            packetLength = _sx127x_read_register(config, REG_PAYLOAD_LENGTH);
        }
        else
        {
            packetLength = _sx127x_read_register(config, REG_RX_NB_uint8_tS);
        }

        _sx127x_write_register(config, REG_FIFO_ADDR_PTR, _sx127x_read_register(config, REG_FIFO_RX_CURRENT_ADDR));

        sx127x_idle(config);
    }
    else if (_sx127x_read_register(config, REG_OP_MODE) != (MODE_LONG_RANGE_MODE | MODE_RX_SINGLE))
    {
        _sx127x_write_register(config, REG_FIFO_ADDR_PTR, 0);
        _sx127x_write_register(config, REG_OP_MODE, MODE_LONG_RANGE_MODE | MODE_RX_SINGLE);
    }

    return packetLength;
}

uint8_t sx127x_read(sx127x_config_t *config)
{
    if (!sx127x_available(config))
    {
        return 0;
    }

    config->packetIndex++;

    return _sx127x_read_register(config, REG_FIFO);
}

uint8_t sx127x_peek(const sx127x_config_t *config)
{
    if (!sx127x_available(config))
    {
        return 0;
    }

    uint8_t currentAddress = _sx127x_read_register(config, REG_FIFO_ADDR_PTR);
    uint8_t d = _sx127x_read_register(config, REG_FIFO);

    _sx127x_write_register(config, REG_FIFO_ADDR_PTR, currentAddress);

    return d;
}

void sx127x_idle(const sx127x_config_t *config)
{
    _sx127x_write_register(config, REG_OP_MODE, MODE_LONG_RANGE_MODE | MODE_STDBY);
}

void sx127x_sleep(const sx127x_config_t *config)
{
    _sx127x_write_register(config, REG_OP_MODE, MODE_LONG_RANGE_MODE | MODE_SLEEP);
}

int sx127x_packet_rssi(const sx127x_config_t *config)
{
    return _sx127x_read_register(config, REG_PKT_RSSI_VALUE) - (config->frequency < RF_MID_BAND_THRESHOLD ? RSSI_OFFSET_LF_PORT : RSSI_OFFSET_HF_PORT);
}

float sx127x_packet_snr(const sx127x_config_t *config)
{
    return ((uint8_t)_sx127x_read_register(config, REG_PKT_SNR_VALUE)) * 0.25f;
}

long sx127x_packet_frequency_error(const sx127x_config_t *config)
{
    int freqError = 0;
    freqError = (int)(_sx127x_read_register(config, REG_FREQ_ERROR_MSB) & 0x111);
    freqError <<= 8L;
    freqError += (int)(_sx127x_read_register(config, REG_FREQ_ERROR_MID));
    freqError <<= 8L;
    freqError += (int)(_sx127x_read_register(config, REG_FREQ_ERROR_LSB));

    if (_sx127x_read_register(config, REG_FREQ_ERROR_MSB) & 0x1000)
    {
        freqError -= 524288;
    }

    const float fXtal = 32E6;
    const float fError = (((float)(freqError) * (1L << 24)) / fXtal) * (_sx127XGetSignalBandwidth(config) / 500000.0f);

    return (long)fError;
}

int sx127x_rssi(const sx127x_config_t *config)
{
    return _sx127x_read_register(config, REG_RSSI_VALUE) - (config->frequency < RF_MID_BAND_THRESHOLD ? RSSI_OFFSET_LF_PORT : RSSI_OFFSET_HF_PORT);
}

void sx127x_set_tx_power(sx127x_config_t *config, int level)
{
    int outputPin = PA_OUTPUT_PA_BOOST_PIN;

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

        _sx127x_write_register(config, REG_PA_CONFIG, 0x70 | level);
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

            _sx127x_write_register(config, REG_PA_DAC, 0x87);
            sx127x_set_ocp(config, 140);
        }
        else
        {
            if (level < 2)
            {
                level = 2;
            }

            _sx127x_write_register(config, REG_PA_DAC, 0x84);
            sx127x_set_ocp(config, 100);
        }

        _sx127x_write_register(config, REG_PA_CONFIG, PA_BOOST | (level - 2));
    }

    config->txPower = level;
}

void sx127x_set_frequency(sx127x_config_t *config, unsigned long long frequency)
{
    config->frequency = frequency;

    unsigned long long frf = (frequency << 19) / 32000000;

    _sx127x_write_register(config, REG_FRF_MSB, (uint8_t)(frf >> 16));
    _sx127x_write_register(config, REG_FRF_MID, (uint8_t)(frf >> 8));
    _sx127x_write_register(config, REG_FRF_LSB, (uint8_t)(frf >> 0));
}

void sx127x_set_spreading_factor(const sx127x_config_t *config, int sf)
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
        _sx127x_write_register(config, REG_DETECTION_OPTIMIZE, 0xc5);
        _sx127x_write_register(config, REG_DETECTION_THRESHOLD, 0x0c);
    }
    else
    {
        _sx127x_write_register(config, REG_DETECTION_OPTIMIZE, 0xc3);
        _sx127x_write_register(config, REG_DETECTION_THRESHOLD, 0x0a);
    }

    _sx127x_write_register(config, REG_MODEM_CONFIG_2, (_sx127x_read_register(config, REG_MODEM_CONFIG_2) & 0x0f) | ((sf << 4) & 0xf0));
    _sx127x_set_ldo_flag(config);
}

void sx127x_set_signal_bandwidth(const sx127x_config_t *config, long sbw)
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

    _sx127x_write_register(config, REG_MODEM_CONFIG_1, (_sx127x_read_register(config, REG_MODEM_CONFIG_1) & 0x0f) | (bw << 4));
    _sx127x_set_ldo_flag(config);
}

void sx127x_set_coding_rate4(const sx127x_config_t *config, int denominator)
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

    _sx127x_write_register(config, REG_MODEM_CONFIG_1, (_sx127x_read_register(config, REG_MODEM_CONFIG_1) & 0xf1) | (cr << 1));
}

void sx127x_set_preamble_length(const sx127x_config_t *config, long length)
{
    _sx127x_write_register(config, REG_PREAMBLE_MSB, (uint8_t)(length >> 8));
    _sx127x_write_register(config, REG_PREAMBLE_LSB, (uint8_t)(length >> 0));
}

void sx127x_set_sync_word(const sx127x_config_t *config, int sw)
{
    _sx127x_write_register(config, REG_SYNC_WORD, sw);
}

void sx127x_enable_crc(const sx127x_config_t *config)
{
    _sx127x_write_register(config, REG_MODEM_CONFIG_2, _sx127x_read_register(config, REG_MODEM_CONFIG_2) | 0x04);
}

void sx127x_disable_crc(const sx127x_config_t *config)
{
    _sx127x_write_register(config, REG_MODEM_CONFIG_2, _sx127x_read_register(config, REG_MODEM_CONFIG_2) & 0xfb);
}

void sx127x_enable_invert_iq(const sx127x_config_t *config)
{
    _sx127x_write_register(config, REG_INVERTIQ, 0x66);
    _sx127x_write_register(config, REG_INVERTIQ2, 0x19);
}

void sx127x_disable_invert_iq(const sx127x_config_t *config)
{
    _sx127x_write_register(config, REG_INVERTIQ, 0x27);
    _sx127x_write_register(config, REG_INVERTIQ2, 0x1d);
}

void sx127x_set_ocp(const sx127x_config_t *config, uint8_t mA)
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

    _sx127x_write_register(config, REG_OCP, 0x20 | (0x1F & ocpTrim));
}

void sx127x_set_gain(const sx127x_config_t *config, uint8_t gain)
{
    if (gain > 6)
    {
        gain = 6;
    }

    sx127x_idle(config);

    if (gain == 0)
    {
        _sx127x_write_register(config, REG_MODEM_CONFIG_3, 0x04);
    }
    else
    {
        _sx127x_write_register(config, REG_MODEM_CONFIG_3, 0x00);
        _sx127x_write_register(config, REG_LNA, 0x03);
        _sx127x_write_register(config, REG_LNA, _sx127x_read_register(config, REG_LNA) | (gain << 5));
    }
}

static void _sx127x_explicit_header_mode(sx127x_config_t *config)
{
    config->implicitHeaderMode = false;

    _sx127x_write_register(config, REG_MODEM_CONFIG_1, _sx127x_read_register(config, REG_MODEM_CONFIG_1) & 0xfe);
}

static void _sx127x_implicit_header_mode(sx127x_config_t *config)
{
    config->implicitHeaderMode = true;

    _sx127x_write_register(config, REG_MODEM_CONFIG_1, _sx127x_read_register(config, REG_MODEM_CONFIG_1) | 0x01);
}

static bool _sx127x_is_transmitting(const sx127x_config_t *config)
{
    if ((_sx127x_read_register(config, REG_OP_MODE) & MODE_TX) == MODE_TX)
    {
        return true;
    }

    if (_sx127x_read_register(config, REG_IRQ_FLAGS) & IRQ_TX_DONE_MASK)
    {
        _sx127x_write_register(config, REG_IRQ_FLAGS, IRQ_TX_DONE_MASK);
    }

    return false;
}

static int _sx127X_get_spreading_factor(const sx127x_config_t *config)
{
    return _sx127x_read_register(config, REG_MODEM_CONFIG_2) >> 4;
}

static long _sx127XGetSignalBandwidth(const sx127x_config_t *config)
{
    uint8_t bw = (_sx127x_read_register(config, REG_MODEM_CONFIG_1) >> 4);

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

static void _sx127x_set_ldo_flag(const sx127x_config_t *config)
{
    long symbolDuration = 1000 / (_sx127XGetSignalBandwidth(config) / (1L << _sx127X_get_spreading_factor(config)));
    bool ldoOn = symbolDuration > 16;
    uint8_t config3 = _sx127x_read_register(config, REG_MODEM_CONFIG_3);

    config3 = ldoOn ? config3 | (1 << 3) : config3 & ~(1 << 3);

    _sx127x_write_register(config, REG_MODEM_CONFIG_3, config3);
}

static uint8_t _sx127x_read_register(const sx127x_config_t *config, uint8_t address)
{
    return _sx127x_single_transfer(config, address & 0x7f, 0x00);
}

static void _sx127x_write_register(const sx127x_config_t *config, uint8_t address, uint8_t value)
{
    _sx127x_single_transfer(config, address | 0x80, value);
}

static uint8_t _sx127x_single_transfer(const sx127x_config_t *config, uint8_t address, uint8_t value)
{
    uint8_t response;

    hal_gpio_set_pin_state(config->cs, GPIO_LOW);

    hal_spi_write(config->spi, &address, 1);
    hal_spi_write_read(config->spi, &value, &response, 1);

    hal_gpio_set_pin_state(config->cs, GPIO_HIGH);

    return response;
}