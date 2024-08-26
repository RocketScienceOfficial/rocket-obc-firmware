#include "lib/drivers/lora/sx126X_driver.h"
#include "hal/time_tracker.h"
#include <string.h>

#define CMD_SET_SLEEP 0x84
#define CMD_SET_STANDBY 0x80
#define CMD_SET_FS 0xC1
#define CMD_SET_TX 0x83
#define CMD_SET_RX 0x82
#define CMD_STOP_TIMER_ON_PREAMBLE 0x9F
#define CMD_SET_RX_DUTY_CYCLE 0x94
#define CMD_SET_CAD 0xC5
#define CMD_SET_TX_CONTINUOUS_WAVE 0xD1
#define CMD_SET_TX_INFINITE_PREAMBLE 0xD2
#define CMD_SET_REGULATOR_MODE 0x96
#define CMD_CALIBRATE 0x89
#define CMD_CALIBRATE_IMAGE 0x98
#define CMD_SET_PA_CONFIG 0x95
#define CMD_SET_RX_TX_FALLBACK_MODE 0x93
#define CMD_WRITE_REGISTER 0x0D
#define CMD_READ_REGISTER 0x1D
#define CMD_WRITE_BUFFER 0x0E
#define CMD_READ_BUFFER 0x1E
#define CMD_SET_DIO_IRQ_PARAMS 0x08
#define CMD_GET_IRQ_STATUS 0x12
#define CMD_CLEAR_IRQ_STATUS 0x02
#define CMD_SET_DIO2_AS_RF_SWITCH_CTRL 0x9D
#define CMD_SET_DIO3_AS_TCXO_CTRL 0x97
#define CMD_SET_RF_FREQUENCY 0x86
#define CMD_SET_PACKET_TYPE 0x8A
#define CMD_GET_PACKET_TYPE 0x11
#define CMD_SET_TX_PARAMS 0x8E
#define CMD_SET_MODULATION_PARAMS 0x8B
#define CMD_SET_PACKET_PARAMS 0x8C
#define CMD_SET_CAD_PARAMS 0x88
#define CMD_SET_BUFFER_BASE_ADDRESS 0x8F
#define CMD_SET_LORA_SYMB_NUM_TIMEOUT 0xA0
#define CMD_GET_STATUS 0xC0
#define CMD_GET_RSSI_INST 0x15
#define CMD_GET_RX_BUFFER_STATUS 0x13
#define CMD_GET_PACKET_STATUS 0x14
#define CMD_GET_DEVICE_ERRORS 0x17
#define CMD_CLEAR_DEVICE_ERRORS 0x07
#define CMD_GET_STATS 0x10
#define CMD_RESET_STATS 0x00

#define REG_DIOX_OUTPUT_ENABLE 0x0580
#define REG_DIOX_INPUT_ENABLE 0x0583
#define REG_DIOX_PULL_UP_CONTROL 0x0584
#define REG_DIOX_PULL_DOWN_CONTROL 0x0585
#define REG_WHITENING_INITIAL_VALUE_MSB 0x06B8
#define REG_WHITENING_INITIAL_VALUE_LSB 0x06B9
#define REG_CRC_MSB_INITIAL_VALUE 0x06BC
#define REG_CRC_LSB_INITIAL_VALUE 0x06BD
#define REG_CRC_MSB_POLYNOMIAL_VALUE 0x06BE
#define REG_CRC_LSB_POLYNOMIAL_VALUE 0x06BF
#define REG_SYNC_WORD_0 0x06C0
#define REG_SYNC_WORD_1 0x06C1
#define REG_SYNC_WORD_2 0x06C2
#define REG_SYNC_WORD_3 0x06C3
#define REG_SYNC_WORD_4 0x06C4
#define REG_SYNC_WORD_5 0x06C5
#define REG_SYNC_WORD_6 0x06C6
#define REG_SYNC_WORD_7 0x06C7
#define REG_NODE_ADDRESS 0x06CD
#define REG_BROADCAST_ADDRESS 0x06CE
#define REG_IQ_POLARITY_SETUP 0x0736
#define REG_LORA_SYNC_WORD_MSB 0x0740
#define REG_LORA_SYNC_WORD_LSB 0x0741
#define REG_RANDOM_NUMBER_GEN_0 0x0819
#define REG_RANDOM_NUMBER_GEN_1 0x081A
#define REG_RANDOM_NUMBER_GEN_2 0x081B
#define REG_RANDOM_NUMBER_GEN_3 0x081C
#define REG_TX_MODULATION 0x0889
#define REG_RX_GAIN 0x08AC
#define REG_TX_CLAMP_CONFIG 0x08D8
#define REG_OCP_CONFIGURATION 0x08E7
#define REG_RTC_CONTROL 0x0902
#define REG_XTA_TRIM 0x0911
#define REG_XTB_TRIM 0x0912
#define DIO3_OUTPUT_VOLTAGE_CONTROL 0x920
#define REG_EVENT_MASK 0x944

#define RTC_FREQ 64000
#define XTAL_FREQ 32000000

static unsigned int _sx126x_convert_timeout_to_rtc_step(unsigned int timeout_ms);
static unsigned int _sx126x_convert_frequency_to_register_value(unsigned int frequency);
static void _sx126x_tx_modulation_workaround(const sx126x_config_t *config, sx126x_packet_type_t packetType, sx126x_lora_bw_t bw);
static void _sx126x_write_register(const sx126x_config_t *config, uint16_t address, uint8_t *buffer, size_t szBuffer);
static void _sx126x_read_register(const sx126x_config_t *config, uint16_t address, uint8_t *buffer, size_t szBuffer);
static void _sx126x_cmd(const sx126x_config_t *config, uint8_t command, uint8_t *params, size_t szParams, uint8_t *resultBuffer, size_t szBuffer);

void sx126x_init(sx126x_config_t *config, hal_spi_instance_t spi, hal_pin_number_t cs, hal_pin_number_t reset, hal_pin_number_t busy)
{
    if (!config)
    {
        return;
    }

    config->spi = spi;
    config->cs = cs;
    config->reset = reset;
    config->busy = busy;

    hal_gpio_init_pin(config->busy, GPIO_INPUT);
    hal_gpio_init_pin(config->reset, GPIO_OUTPUT);

    hal_spi_init_cs(config->cs);

    sx126x_reset(config);
}

void sx126x_set_sleep(const sx126x_config_t *config, bool coldStart)
{
    uint8_t buff[1];

    buff[0] = (uint8_t)(coldStart ? 0 : (1 << 2));

    _sx126x_cmd(config, CMD_SET_SLEEP, buff, 1, NULL, 0);
}

void sx126x_set_standby(const sx126x_config_t *config, sx126x_standby_mode_t mode)
{
    uint8_t buff[1];

    buff[0] = (uint8_t)mode;

    _sx126x_cmd(config, CMD_SET_STANDBY, buff, 1, NULL, 0);
}

void sx126x_set_fs(const sx126x_config_t *config)
{
    _sx126x_cmd(config, CMD_SET_FS, NULL, 0, NULL, 0);
}

void sx126x_set_tx(const sx126x_config_t *config, unsigned int timeout_ms)
{
    uint8_t buff[3];

    unsigned int timeout = _sx126x_convert_timeout_to_rtc_step(timeout_ms);

    buff[0] = (uint8_t)(timeout >> 16);
    buff[1] = (uint8_t)(timeout >> 8);
    buff[2] = (uint8_t)(timeout);

    _sx126x_cmd(config, CMD_SET_TX, buff, 3, NULL, 0);
}

void sx126x_set_rx(const sx126x_config_t *config, unsigned int timeout_ms)
{
    uint8_t buff[3];

    unsigned int timeout = _sx126x_convert_timeout_to_rtc_step(timeout_ms);

    buff[0] = (uint8_t)(timeout >> 16);
    buff[1] = (uint8_t)(timeout >> 8);
    buff[2] = (uint8_t)(timeout);

    _sx126x_cmd(config, CMD_SET_RX, buff, 3, NULL, 0);
}

void sx126x_stop_timer_on_preamble(const sx126x_config_t *config, bool enable)
{
    uint8_t buff[1];

    buff[0] = (uint8_t)enable;

    _sx126x_cmd(config, CMD_STOP_TIMER_ON_PREAMBLE, buff, 1, NULL, 0);
}

void sx126x_set_rx_duty_cycle(const sx126x_config_t *config, unsigned int rxPeriod_ms, unsigned int sleepPeriod_ms)
{
    uint8_t buff[6];

    unsigned int rxPeriod = _sx126x_convert_timeout_to_rtc_step(rxPeriod_ms);
    unsigned int sleepPeriod = _sx126x_convert_timeout_to_rtc_step(sleepPeriod_ms);

    buff[0] = (uint8_t)(rxPeriod >> 16);
    buff[1] = (uint8_t)(rxPeriod >> 8);
    buff[2] = (uint8_t)(rxPeriod);
    buff[3] = (uint8_t)(sleepPeriod >> 16);
    buff[4] = (uint8_t)(sleepPeriod >> 8);
    buff[5] = (uint8_t)(sleepPeriod);

    _sx126x_cmd(config, CMD_SET_RX_DUTY_CYCLE, buff, 6, NULL, 0);
}

void sx126x_set_cad(const sx126x_config_t *config)
{
    _sx126x_cmd(config, CMD_SET_CAD, NULL, 0, NULL, 0);
}

void sx126x_set_tx_continuous_wave(const sx126x_config_t *config)
{
    _sx126x_cmd(config, CMD_SET_TX_CONTINUOUS_WAVE, NULL, 0, NULL, 0);
}

void sx126x_set_tx_infinite_preamble(const sx126x_config_t *config)
{
    _sx126x_cmd(config, CMD_SET_TX_INFINITE_PREAMBLE, NULL, 0, NULL, 0);
}

void sx126x_set_regulator_mode(const sx126x_config_t *config, sx126x_regulator_mode_t mode)
{
    uint8_t buff[1];

    buff[0] = (uint8_t)mode;

    _sx126x_cmd(config, CMD_SET_REGULATOR_MODE, buff, 1, NULL, 0);
}

void sx126x_calibrate(const sx126x_config_t *config, uint8_t calibParam)
{
    uint8_t buff[1];

    buff[0] = calibParam;

    _sx126x_cmd(config, CMD_CALIBRATE, buff, 1, NULL, 0);
}

void sx126x_calibrate_image(const sx126x_config_t *config, unsigned int freq_hz)
{
    uint8_t buff[2];

    if (freq_hz >= 430E6 && freq_hz <= 440E6)
    {
        buff[0] = 0x6B;
        buff[1] = 0x6F;
    }
    else if (freq_hz >= 470E6 && freq_hz <= 510E6)
    {
        buff[0] = 0x75;
        buff[1] = 0x81;
    }
    else if (freq_hz >= 779E6 && freq_hz <= 787E6)
    {
        buff[0] = 0xC1;
        buff[1] = 0xC5;
    }
    else
    {
        return;
    }

    _sx126x_cmd(config, CMD_CALIBRATE_IMAGE, buff, 2, NULL, 0);
}

void sx126x_set_pa_config(const sx126x_config_t *config, uint8_t paDutyCycle, uint8_t hpMax)
{
    uint8_t buff[4];

    buff[0] = paDutyCycle;
    buff[1] = hpMax;
    buff[2] = 0x00;
    buff[3] = 0x01;

    _sx126x_cmd(config, CMD_SET_PA_CONFIG, buff, 4, NULL, 0);
}

void sx126x_set_rx_tx_fallback_mode(const sx126x_config_t *config, sx126x_fallback_mode_t fallbackMode)
{
    uint8_t buff[1];

    buff[0] = (uint8_t)fallbackMode;

    _sx126x_cmd(config, CMD_SET_RX_TX_FALLBACK_MODE, buff, 1, NULL, 0);
}

void sx126x_write_buffer(const sx126x_config_t *config, uint8_t offset, uint8_t *buffer, size_t szBuffer)
{
    uint8_t buff[16];

    buff[0] = (uint8_t)offset;

    memcpy(buff + 1, buffer, szBuffer);

    _sx126x_cmd(config, CMD_WRITE_BUFFER, buff, szBuffer + 1, NULL, 0);
}

void sx126x_read_buffer(const sx126x_config_t *config, uint8_t offset, uint8_t *buffer, size_t szBuffer)
{
    uint8_t buff[1];

    buff[0] = (uint8_t)offset;

    uint8_t resBuff[16];

    _sx126x_cmd(config, CMD_READ_BUFFER, buff, 1, resBuff, szBuffer + 1);

    memcpy(buffer, resBuff + 1, szBuffer);
}

void sx126x_set_dio_irq_params(const sx126x_config_t *config, uint16_t irqMask, uint16_t dio1Mask, uint16_t dio2Mask, uint16_t dio3Mask)
{
    uint8_t buff[8];

    buff[0] = (uint8_t)(irqMask >> 8);
    buff[1] = (uint8_t)(irqMask);
    buff[2] = (uint8_t)(dio1Mask >> 8);
    buff[3] = (uint8_t)(dio1Mask);
    buff[4] = (uint8_t)(dio2Mask >> 8);
    buff[5] = (uint8_t)(dio2Mask);
    buff[6] = (uint8_t)(dio3Mask >> 8);
    buff[7] = (uint8_t)(dio3Mask);

    _sx126x_cmd(config, CMD_SET_DIO_IRQ_PARAMS, buff, 8, NULL, 0);
}

void sx126x_get_irq_status(const sx126x_config_t *config, uint16_t *irqStatus)
{
    uint8_t buff[3];

    _sx126x_cmd(config, CMD_GET_IRQ_STATUS, NULL, 0, buff, 3);

    *irqStatus = (uint16_t)((buff[1] << 8) | buff[2]);
}

void sx126x_clear_irq_status(const sx126x_config_t *config, uint16_t clearIrqParam)
{
    uint8_t buff[2];

    buff[0] = (uint8_t)(clearIrqParam >> 8);
    buff[1] = (uint8_t)(clearIrqParam);

    _sx126x_cmd(config, CMD_CLEAR_IRQ_STATUS, buff, 2, NULL, 0);
}

void sx126x_set_dio2_as_rf_switch_ctrl(const sx126x_config_t *config, bool enable)
{
    uint8_t buff[1];

    buff[0] = (uint8_t)enable;

    _sx126x_cmd(config, CMD_SET_DIO2_AS_RF_SWITCH_CTRL, buff, 1, NULL, 0);
}

void sx126x_set_dio3_as_tcxo_ctrl(const sx126x_config_t *config, sx126x_tcxo_voltage_t voltage, unsigned int timeout_ms)
{
    uint8_t buff[4];

    unsigned int timeout = _sx126x_convert_timeout_to_rtc_step(timeout_ms);

    buff[0] = (uint8_t)voltage;
    buff[1] = (uint8_t)(timeout >> 16);
    buff[2] = (uint8_t)(timeout >> 8);
    buff[3] = (uint8_t)(timeout);

    _sx126x_cmd(config, CMD_SET_DIO3_AS_TCXO_CTRL, buff, 3, NULL, 0);
}

void sx126x_set_rf_frequency(const sx126x_config_t *config, unsigned int frequency)
{
    uint8_t buff[4];

    unsigned int freq = _sx126x_convert_frequency_to_register_value(frequency);

    buff[0] = (uint8_t)(freq >> 24);
    buff[1] = (uint8_t)(freq >> 16);
    buff[2] = (uint8_t)(freq >> 8);
    buff[3] = (uint8_t)(freq);

    _sx126x_cmd(config, CMD_SET_RF_FREQUENCY, buff, 4, NULL, 0);
}

void sx126x_set_packet_type(const sx126x_config_t *config, sx126x_packet_type_t packetType)
{
    uint8_t buff[1];

    buff[0] = (uint8_t)packetType;

    _sx126x_cmd(config, CMD_SET_PACKET_TYPE, buff, 1, NULL, 0);
}

void sx126x_get_packet_type(const sx126x_config_t *config, sx126x_packet_type_t *packetType)
{
    uint8_t buff[2];

    _sx126x_cmd(config, CMD_GET_PACKET_TYPE, NULL, 0, buff, 2);

    *packetType = (sx126x_packet_type_t)buff[1];
}

void sx126x_set_tx_params(const sx126x_config_t *config, int8_t power, sx126x_ramp_time_t rampTime)
{
    uint8_t buff[2];

    buff[0] = (uint8_t)power;
    buff[1] = (uint8_t)rampTime;

    _sx126x_cmd(config, CMD_SET_TX_PARAMS, buff, 2, NULL, 0);
}

void sx126x_set_gfsk_modulation_params(const sx126x_config_t *config, unsigned int bitrate, sx126x_gfsk_pulse_shape_t pulseShape, sx126x_gfsk_bandwidth_t bandwidth, unsigned int freq_dev_hz)
{
    uint8_t buff[8];

    unsigned int br = (unsigned int)(32 * (double)XTAL_FREQ / (double)bitrate);
    unsigned int freq_dev = _sx126x_convert_frequency_to_register_value(freq_dev_hz);

    buff[0] = (uint8_t)(br >> 16);
    buff[1] = (uint8_t)(br >> 8);
    buff[2] = (uint8_t)(br);
    buff[3] = (uint8_t)pulseShape;
    buff[4] = (uint8_t)bandwidth;
    buff[5] = (uint8_t)(freq_dev >> 16);
    buff[6] = (uint8_t)(freq_dev >> 8);
    buff[7] = (uint8_t)(freq_dev);

    _sx126x_cmd(config, CMD_SET_MODULATION_PARAMS, buff, 8, NULL, 0);

    _sx126x_tx_modulation_workaround(config, SX126X_PACKET_TYPE_LORA, 0);
}

void sx126x_set_lora_modulation_params(const sx126x_config_t *config, sx126x_lora_sf_t sf, sx126x_lora_bw_t bw, sx126x_lora_cr_t cr, bool lowconfigRateOptimize)
{
    uint8_t buff[4];

    buff[0] = (uint8_t)sf;
    buff[1] = (uint8_t)bw;
    buff[2] = (uint8_t)cr;
    buff[3] = (uint8_t)lowconfigRateOptimize;

    _sx126x_cmd(config, CMD_SET_PACKET_PARAMS, buff, 4, NULL, 0);

    _sx126x_tx_modulation_workaround(config, SX126X_PACKET_TYPE_LORA, bw);
}

void sx126x_set_packet_gfsk_params(const sx126x_config_t *config, uint16_t preambleLength, sx126x_gfsk_preamble_detector_length_t detectorLength, uint8_t syncWordLength, sx126x_gfsk_address_filtering_t addressFiltering, sx126x_gfsk_packet_type_t packetType, uint8_t payloadLength, sx126x_gfsk_crc_type_t crcType, bool whitening)
{
    uint8_t buff[9];

    buff[0] = (uint8_t)(preambleLength >> 8);
    buff[1] = (uint8_t)(preambleLength);
    buff[2] = (uint8_t)detectorLength;
    buff[3] = (uint8_t)syncWordLength;
    buff[4] = (uint8_t)addressFiltering;
    buff[5] = (uint8_t)packetType;
    buff[6] = (uint8_t)payloadLength;
    buff[7] = (uint8_t)crcType;
    buff[8] = (uint8_t)whitening;

    _sx126x_cmd(config, CMD_SET_PACKET_PARAMS, buff, 9, NULL, 0);
}

void sx126x_set_packet_lora_params(const sx126x_config_t *config, uint16_t preambleLength, sx126x_lora_header_type_t headerType, uint8_t payloadLength, bool crc, bool invertIQ)
{
    uint8_t buff[6];

    buff[0] = (uint8_t)(preambleLength >> 8);
    buff[1] = (uint8_t)(preambleLength);
    buff[2] = (uint8_t)headerType;
    buff[3] = (uint8_t)payloadLength;
    buff[4] = (uint8_t)crc;
    buff[5] = (uint8_t)invertIQ;

    _sx126x_cmd(config, CMD_SET_PACKET_PARAMS, buff, 6, NULL, 0);

    uint8_t reg;
    _sx126x_read_register(config, REG_IQ_POLARITY_SETUP, &reg, 1);

    if (invertIQ)
    {
        reg &= ~(1 << 2);
    }
    else
    {
        reg |= (1 << 2);
    }

    _sx126x_write_register(config, REG_IQ_POLARITY_SETUP, &reg, 1);
}

void sx126x_set_cad_params(const sx126x_config_t *config, sx126x_cad_symbol_t cadSymbolNum, uint8_t cadDetPeak, uint8_t cadDetMin, sx126x_lora_cad_exit_mode_t cadExitMode, unsigned int timeout_ms)
{
    uint8_t buff[7];

    unsigned int timeout = _sx126x_convert_timeout_to_rtc_step(timeout_ms);

    buff[0] = (uint8_t)cadSymbolNum;
    buff[1] = (uint8_t)cadDetPeak;
    buff[2] = (uint8_t)cadDetMin;
    buff[3] = (uint8_t)cadExitMode;
    buff[4] = (uint8_t)(timeout >> 16);
    buff[5] = (uint8_t)(timeout >> 8);
    buff[6] = (uint8_t)(timeout);

    _sx126x_cmd(config, CMD_SET_CAD_PARAMS, buff, 7, NULL, 0);
}

void sx126x_set_buffer_base_address(const sx126x_config_t *config, uint8_t txBaseAddress, uint8_t rxBaseAddress)
{
    uint8_t buff[2];

    buff[0] = (uint8_t)txBaseAddress;
    buff[1] = (uint8_t)rxBaseAddress;

    _sx126x_cmd(config, CMD_SET_BUFFER_BASE_ADDRESS, buff, 2, NULL, 0);
}

void sx126x_set_lora_symb_num_timeout(const sx126x_config_t *config, uint8_t symbNum)
{
    uint8_t buff[1];

    buff[0] = (uint8_t)symbNum;

    _sx126x_cmd(config, CMD_SET_LORA_SYMB_NUM_TIMEOUT, buff, 1, NULL, 0);
}

void sx126x_get_status(const sx126x_config_t *config, sx126x_chip_mode_t *pChipMode, sx126x_command_status_t *pCmdStatus)
{
    uint8_t buff[1];

    _sx126x_cmd(config, CMD_GET_STATUS, NULL, 0, buff, 1);

    uint8_t d = buff[0];

    *pChipMode = (sx126x_chip_mode_t)((d & 0x70) >> 4);
    *pCmdStatus = (sx126x_command_status_t)((d & 0x0E) >> 1);
}

void sx126x_get_rx_buffer_status(const sx126x_config_t *config, uint8_t *pPayloadLength, uint8_t *pRXStartBufferPtr)
{
    uint8_t buff[3];

    _sx126x_cmd(config, CMD_GET_RX_BUFFER_STATUS, NULL, 0, buff, 3);

    *pPayloadLength = buff[1];
    *pRXStartBufferPtr = buff[2];
}

void sx126x_get_gfsk_packet_status(const sx126x_config_t *config, sx126x_gfsk_packet_rx_status_t *pRXStatus, int8_t *pRssiSync, int8_t *pRssiAvg)
{
    uint8_t buff[4];

    _sx126x_cmd(config, CMD_GET_PACKET_STATUS, NULL, 0, buff, 4);

    memcpy(pRXStatus, &buff[1], 1);

    *pRssiSync = (int8_t)(-buff[2] >> 1);
    *pRssiAvg = (int8_t)(-buff[3] >> 1);
}

void sx126x_get_lora_packet_status(const sx126x_config_t *config, int8_t *pRssiPacket, int8_t *pSnrPkt, int8_t *pSignalRssiPkt)
{
    uint8_t buff[4];

    _sx126x_cmd(config, CMD_GET_PACKET_STATUS, NULL, 0, buff, 4);

    *pRssiPacket = (int8_t)(-buff[1] >> 1);
    *pSnrPkt = (int8_t)(buff[2] >> 2);
    *pSignalRssiPkt = (int8_t)(-buff[3] >> 1);
}

void sx126x_get_rssi_inst(const sx126x_config_t *config, int8_t *pRssiInst)
{
    uint8_t buff[2];

    _sx126x_cmd(config, CMD_GET_RSSI_INST, NULL, 0, buff, 2);

    *pRssiInst = (int8_t)(-buff[1] >> 1);
}

void sx126x_get_gfsk_stats(const sx126x_config_t *config, uint16_t *pNbPktReceived, uint16_t *pNbPktCrcErr, uint16_t *pNbPktLenErr)
{
    uint8_t buff[7];

    _sx126x_cmd(config, CMD_GET_STATS, NULL, 0, buff, 7);

    *pNbPktReceived = (uint16_t)((buff[1] << 8) | buff[2]);
    *pNbPktCrcErr = (uint16_t)((buff[3] << 8) | buff[4]);
    *pNbPktLenErr = (uint16_t)((buff[5] << 8) | buff[6]);
}

void sx126x_get_lora_stats(const sx126x_config_t *config, uint16_t *pNbPktReceived, uint16_t *pNbPktCrcErr, uint16_t *pNbPktHeaderErr)
{
    uint8_t buff[7];

    _sx126x_cmd(config, CMD_GET_STATS, NULL, 0, buff, 7);

    *pNbPktReceived = (uint16_t)((buff[1] << 8) | buff[2]);
    *pNbPktCrcErr = (uint16_t)((buff[3] << 8) | buff[4]);
    *pNbPktHeaderErr = (uint16_t)((buff[5] << 8) | buff[6]);
}

void sx126x_reset_stats(const sx126x_config_t *config)
{
    uint8_t buff[6];
    memset(buff, 0, 6);

    _sx126x_cmd(config, CMD_RESET_STATS, buff, 6, NULL, 0);
}

void sx126x_get_device_errors(const sx126x_config_t *config, uint16_t *pErrors)
{
    uint8_t buff[3];

    _sx126x_cmd(config, CMD_GET_DEVICE_ERRORS, NULL, 0, buff, 3);

    *pErrors = (uint16_t)((buff[1] << 8) | buff[2]);
}

void sx126x_clear_device_errors(const sx126x_config_t *config)
{
    uint8_t buff[2];
    memset(buff, 0, 2);

    _sx126x_cmd(config, CMD_CLEAR_DEVICE_ERRORS, buff, 2, NULL, 0);
}

void sx126x_is_busy(const sx126x_config_t *config, bool *pStatus)
{
    *pStatus = hal_gpio_get_pin_state(config->busy) == GPIO_HIGH;
}

void sx126x_check_busy(const sx126x_config_t *config)
{
    bool busy;
    sx126x_is_busy(config, &busy);

    while (busy)
    {
        sx126x_is_busy(config, &busy);
        hal_time_sleep_ms(1);
    }
}

void sx126x_reset(const sx126x_config_t *config)
{
    hal_time_sleep_ms(20);
    hal_gpio_set_pin_state(config->reset, GPIO_LOW);
    hal_time_sleep_ms(50);
    hal_gpio_set_pin_state(config->reset, GPIO_HIGH);
    hal_time_sleep_ms(20);
}

void sx126x_wakeup(const sx126x_config_t *config)
{
    hal_gpio_set_pin_state(config->cs, GPIO_LOW);
    hal_time_sleep_ms(2);
    hal_gpio_set_pin_state(config->cs, GPIO_HIGH);
    hal_time_sleep_ms(2);
}

void sx126x_clamp_tx(const sx126x_config_t *config)
{
    uint8_t reg;

    _sx126x_read_register(config, REG_TX_CLAMP_CONFIG, &reg, 1);

    reg |= 0x1E;

    _sx126x_write_register(config, REG_TX_CLAMP_CONFIG, &reg, 1);
}

void sx126x_stop_rtc(const sx126x_config_t *config)
{
    uint8_t reg = 0;

    _sx126x_write_register(config, REG_RTC_CONTROL, &reg, 1);
    _sx126x_read_register(config, REG_RTC_CONTROL, &reg, 1);

    reg |= 0x02;

    _sx126x_write_register(config, REG_RTC_CONTROL, &reg, 1);
}

static unsigned int _sx126x_convert_timeout_to_rtc_step(unsigned int timeout_ms)
{
    return timeout_ms * RTC_FREQ / 1000;
}

static unsigned int _sx126x_convert_frequency_to_register_value(unsigned int frequency)
{
    return (unsigned int)((frequency / (double)XTAL_FREQ) * (double)(1 << 25));
}

static void _sx126x_tx_modulation_workaround(const sx126x_config_t *config, sx126x_packet_type_t packetType, sx126x_lora_bw_t bw)
{
    uint8_t reg;

    _sx126x_read_register(config, REG_TX_MODULATION, &reg, 1);

    if (packetType == SX126X_PACKET_TYPE_GFSK)
    {
        reg |= (1 << 2);
    }
    else
    {
        if (bw == SX126X_LORA_BW_500)
        {
            reg &= ~(1 << 2);
        }
        else
        {
            reg |= (1 << 2);
        }
    }

    _sx126x_write_register(config, REG_TX_MODULATION, &reg, 1);
}

static void _sx126x_write_register(const sx126x_config_t *config, uint16_t address, uint8_t *buffer, size_t szBuffer)
{
    uint8_t buff[16];

    buff[0] = (uint8_t)(address >> 8);
    buff[1] = (uint8_t)(address);

    memcpy(buff + 2, buffer, szBuffer);

    _sx126x_cmd(config, CMD_WRITE_REGISTER, buff, szBuffer + 2, NULL, 0);
}

static void _sx126x_read_register(const sx126x_config_t *config, uint16_t address, uint8_t *buffer, size_t szBuffer)
{
    _sx126x_cmd(config, CMD_READ_REGISTER, (uint8_t *)&address, 2, buffer, szBuffer);
}

static void _sx126x_cmd(const sx126x_config_t *config, uint8_t command, uint8_t *params, size_t szParams, uint8_t *resultBuffer, size_t szBuffer)
{
    sx126x_check_busy(config);

    uint8_t buff[16];

    buff[0] = command;

    if (params && szParams > 0)
    {
        memcpy(buff + 1, params, szParams);
    }

    hal_spi_cs_select(config->cs);

    hal_spi_write(config->spi, buff, szParams + 1);

    if (resultBuffer && szBuffer > 0)
    {
        hal_spi_read(config->spi, 0, resultBuffer, szBuffer);
    }

    hal_spi_cs_select(config->cs);
}