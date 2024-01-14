#ifndef _SX126X_DRIVER_H
#define _SX126X_DRIVER_H

#include "modules/drivers/hal/gpio_driver.h"
#include "modules/drivers/hal/spi_driver.h"
#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

/**
 * @brief Pinout data to use in the radio
 */
typedef struct sx126x_pinout
{
    hal_spi_instance_t spi; /** SPI */
    hal_pin_number_t cs;    /** CS */
    hal_pin_number_t reset; /** RESET */
    hal_pin_number_t busy;  /** BUSY */
} sx126x_pinout_t;

/**
 * @brief LoRa radio configuration. All changes are done internally, so keep it just for reference!
 */
typedef struct sx126x_config
{
    sx126x_pinout_t pinout; /** Pinout */
} sx126x_config_t;

/**
 * @brief SX126X GFSK packet RX status structure
 */
typedef struct sx126x_gfsk_packet_rx_status
{
    bool preambleErr;
    bool syncErr;
    bool addrErr;
    bool crcErr;
    bool lengthErr;
    bool abortErr;
    bool packetReceived;
    bool packetSent;
} sx126x_gfsk_packet_rx_status_t;

/**
 * @brief SX126X Standy mode
 */
typedef enum sx126x_standby_mode
{
    SX126X_STANDBY_RC = 0x00,
    SX126X_STANDBY_XOSC = 0x01,
} sx126x_standby_mode_t;

/**
 * @brief SX126X Regulator mode
 */
typedef enum sx126x_regulator_mode
{
    SX126X_REGULATOR_LDO = 0x00,
    SX126X_REGULATOR_DC_DC = 0x01,
} sx126x_regulator_mode_t;

/**
 * @brief SX126X Fallback mode
 */
typedef enum sx126x_fallback_mode
{
    SX126X_FALLBACK_MODE_STDBY_RC = 0x20,
    SX126X_FALLBACK_MODE_STDBY_XOSC = 0x30,
    SX126X_FALLBACK_MODE_FS = 0x40,
} sx126x_fallback_mode_t;

/**
 * @brief SX126X IRQ mask
 */
typedef enum sx126x_irq_mask
{
    SX126X_IRQ_TX_DONE_MASK = (1 << 0),
    SX126X_IRQ_RX_DONE_MASK = (1 << 1),
    SX126X_IRQ_PREAMBLE_DETECTED_MASK = (1 << 2),
    SX126X_IRQ_SYNC_WORD_VALID_MASK = (1 << 3),
    SX126X_IRQ_HEADER_VALID_MASK = (1 << 4),
    SX126X_IRQ_HEADER_ERR_MASK = (1 << 5),
    SX126X_IRQ_CRC_ERR_MASK = (1 << 6),
    SX126X_IRQ_CAD_DONE_MASK = (1 << 7),
    SX126X_IRQ_CAD_ACTIVITY_DETECTED_MASK = (1 << 8),
    SX126X_IRQ_RX_TX_TIMEOUT_MASK = (1 << 9),
    SX126X_IRQ_RADIO_ALL_MASK = SX126X_IRQ_TX_DONE_MASK | SX126X_IRQ_RX_DONE_MASK | SX126X_IRQ_PREAMBLE_DETECTED_MASK | SX126X_IRQ_SYNC_WORD_VALID_MASK | SX126X_IRQ_HEADER_VALID_MASK | SX126X_IRQ_HEADER_ERR_MASK | SX126X_IRQ_CRC_ERR_MASK | SX126X_IRQ_CAD_DONE_MASK | SX126X_IRQ_CAD_ACTIVITY_DETECTED_MASK | SX126X_IRQ_RX_TX_TIMEOUT_MASK,
} sx126x_irq_mask_t;

/**
 * @brief SX126X Calibration Mask
 */
typedef enum sx126x_calibration_mask
{
    SX126X_CALIBRATE_RC64K_MASK = (1 << 0),
    SX126X_CALIBRATE_RC13M_MASK = (1 << 1),
    SX126X_CALIBRATE_PLL_MASK = (1 << 2),
    SX126X_CALIBRATE_ADC_PULSE_MASK = (1 << 3),
    SX126X_CALIBRATE_ADC_BULK_N_MASK = (1 << 4),
    SX126X_CALIBRATE_ADC_BULK_P_MASK = (1 << 5),
    SX126X_CALIBRATE_IMAGE_MASK = (1 << 6),
    SX126X_CALIBRATE_ALL_MASK = SX126X_CALIBRATE_RC64K_MASK | SX126X_CALIBRATE_RC13M_MASK | SX126X_CALIBRATE_PLL_MASK | SX126X_CALIBRATE_ADC_PULSE_MASK | SX126X_CALIBRATE_ADC_BULK_N_MASK | SX126X_CALIBRATE_ADC_BULK_P_MASK | SX126X_CALIBRATE_IMAGE_MASK,
} sx126x_calibration_mask_t;

/**
 * @brief SX126X TCXO voltage
 */
typedef enum sx126x_tcxo_voltage
{
    SX126X_TCXO_CTRL_1_6V = 0x00,
    SX126X_TCXO_CTRL_1_7V = 0x01,
    SX126X_TCXO_CTRL_1_8V = 0x02,
    SX126X_TCXO_CTRL_2_2V = 0x03,
    SX126X_TCXO_CTRL_2_4V = 0x04,
    SX126X_TCXO_CTRL_2_7V = 0x05,
    SX126X_TCXO_CTRL_3_0V = 0x06,
    SX126X_TCXO_CTRL_3_3V = 0x07,
} sx126x_tcxo_voltage_t;

/**
 * @brief SX126X Ramp Time
 */
typedef enum sx126x_ramp_time
{
    SX126X_RAMP_10U = 0x00,
    SX126X_RAMP_20U = 0x01,
    SX126X_RAMP_40U = 0x02,
    SX126X_RAMP_80U = 0x03,
    SX126X_RAMP_200U = 0x04,
    SX126X_RAMP_800U = 0x05,
    SX126X_RAMP_1700U = 0x06,
    SX126X_RAMP_3400U = 0x07,
} sx126x_ramp_time_t;

/**
 * @brief SX126X Packet Type
 */
typedef enum sx126x_packet_type
{
    SX126X_PACKET_TYPE_GFSK = 0x00,
    SX126X_PACKET_TYPE_LORA = 0x01,
} sx126x_packet_type_t;

/**
 * @brief SX126X GFSK Pulse Shape
 */
typedef enum sx126x_gfsk_pulse_shape
{
    SX126X_GFSK_PULSE_SHAPE_OFF = 0x00,
    SX126X_GFSK_PULSE_SHAPE_BT_03 = 0x08,
    SX126X_GFSK_PULSE_SHAPE_BT_05 = 0x09,
    SX126X_GFSK_PULSE_SHAPE_BT_07 = 0x0A,
    SX126X_GFSK_PULSE_SHAPE_BT_1 = 0x0B,
} sx126x_gfsk_pulse_shape_t;

/**
 * @brief SX126X GFSK Bandwidth
 */
typedef enum sx126x_gfsk_bandwidth
{
    SX126X_GFSK_BW_4800 = 0x1F,
    SX126X_GFSK_BW_5800 = 0x17,
    SX126X_GFSK_BW_7300 = 0x0F,
    SX126X_GFSK_BW_9700 = 0x1E,
    SX126X_GFSK_BW_11700 = 0x16,
    SX126X_GFSK_BW_14600 = 0x0E,
    SX126X_GFSK_BW_19500 = 0x1D,
    SX126X_GFSK_BW_23400 = 0x15,
    SX126X_GFSK_BW_29300 = 0x0D,
    SX126X_GFSK_BW_39000 = 0x1C,
    SX126X_GFSK_BW_46900 = 0x14,
    SX126X_GFSK_BW_58600 = 0x0C,
    SX126X_GFSK_BW_78200 = 0x1B,
    SX126X_GFSK_BW_93800 = 0x13,
    SX126X_GFSK_BW_117300 = 0x0B,
    SX126X_GFSK_BW_156200 = 0x1A,
    SX126X_GFSK_BW_187200 = 0x12,
    SX126X_GFSK_BW_234300 = 0x0A,
    SX126X_GFSK_BW_312000 = 0x19,
    SX126X_GFSK_BW_373600 = 0x11,
    SX126X_GFSK_BW_467000 = 0x09,
} sx126x_gfsk_bandwidth_t;

/**
 * @brief SX126X LoRa Spreading Factor
 */
typedef enum sx126x_lora_sf
{
    SX126X_LORA_SF_5 = 0x05,
    SX126X_LORA_SF_6 = 0x06,
    SX126X_LORA_SF_7 = 0x07,
    SX126X_LORA_SF_8 = 0x08,
    SX126X_LORA_SF_9 = 0x09,
    SX126X_LORA_SF_10 = 0x0A,
    SX126X_LORA_SF_11 = 0x0B,
    SX126X_LORA_SF_12 = 0x0C,
} sx126x_lora_sf_t;

/**
 * @brief SX126X LoRa Bandwidth
 */
typedef enum sx126x_lora_bw
{
    SX126X_LORA_BW_7_8 = 0x00,
    SX126X_LORA_BW_10_4 = 0x01,
    SX126X_LORA_BW_15_6 = 0x02,
    SX126X_LORA_BW_20_8 = 0x03,
    SX126X_LORA_BW_31_25 = 0x04,
    SX126X_LORA_BW_41_7 = 0x05,
    SX126X_LORA_BW_62_5 = 0x06,
    SX126X_LORA_BW_125 = 0x07,
    SX126X_LORA_BW_250 = 0x08,
    SX126X_LORA_BW_500 = 0x09,
} sx126x_lora_bw_t;

/**
 * @brief SX126X LoRa Coding Rate
 */
typedef enum sx126x_lora_cr
{
    SX126X_LORA_CR_4_5 = 0x01,
    SX126X_LORA_CR_4_6 = 0x02,
    SX126X_LORA_CR_4_7 = 0x03,
    SX126X_LORA_CR_4_8 = 0x04,
} sx126x_lora_cr_t;

/**
 * @brief SX126X GFSK Preamble Detector Length
 */
typedef enum sx126x_gfsk_preamble_detector_length
{
    SX126X_GFSK_PREAMBLE_DETECTOR_LENGTH_OFF = 0x00,
    SX126X_GFSK_PREAMBLE_DETECTOR_LENGTH_08B = 0x04,
    SX126X_GFSK_PREAMBLE_DETECTOR_LENGTH_16B = 0x05,
    SX126X_GFSK_PREAMBLE_DETECTOR_LENGTH_24B = 0x06,
    SX126X_GFSK_PREAMBLE_DETECTOR_LENGTH_32B = 0x07,
} sx126x_gfsk_preamble_detector_length_t;

/**
 * @brief SX126X GFSK Address Filtering
 */
typedef enum sx126x_gfsk_address_filtering
{
    SX126X_GFSK_ADDRESS_FILTERING_OFF = 0x00,
    SX126X_GFSK_ADDRESS_FILTERING_NODE = 0x01,
    SX126X_GFSK_ADDRESS_FILTERING_NODE_BROADCAST = 0x02,
} sx126x_gfsk_address_filtering_t;

/**
 * @brief SX126X GFSK Packet Type
 */
typedef enum sx126x_gfsk_packet_type
{
    SX126X_GFSK_PACKET_TYPE_VAR_LEN = 0x00,
    SX126X_GFSK_PACKET_TYPE_FIX_LEN = 0x01,
} sx126x_gfsk_packet_type_t;

/**
 * @brief SX126X GFSK CRC Type
 */
typedef enum sx126x_gfsk_crc_type
{
    SX126X_GFSK_CRC_OFF = 0x01,
    SX126X_GFSK_CRC_1_uint8_t = 0x00,
    SX126X_GFSK_CRC_2_uint8_t = 0x02,
    SX126X_GFSK_CRC_1_uint8_t_INV = 0x04,
    SX126X_GFSK_CRC_2_uint8_t_INV = 0x06,
} sx126x_gfsk_crc_type_t;

/**
 * @brief SX126X LoRa Header Type
 */
typedef enum sx126x_lora_header_type
{
    SX126X_LORA_HEADER_IMPLICIT = 0x00,
    SX126X_LORA_HEADER_EXPLICIT = 0x01,
} sx126x_lora_header_type_t;

/**
 * @brief SX126X CAD Symbol
 */
typedef enum sx126x_cad_symbol
{
    SX126X_CAD_SYMBOL_1 = 0x00,
    SX126X_CAD_SYMBOL_2 = 0x01,
    SX126X_CAD_SYMBOL_4 = 0x02,
    SX126X_CAD_SYMBOL_8 = 0x03,
    SX126X_CAD_SYMBOL_16 = 0x04,
} sx126x_cad_symbol_t;

/**
 * @brief SX126X CAD Exit Mode
 */
typedef enum sx126x_cad_exit_mode
{
    SX126X_CAD_ONLY = 0x00,
    SX126X_CAD_RX = 0x01,
} sx126x_lora_cad_exit_mode_t;

/**
 * @brief SX126X Chip Mode
 */
typedef enum sx126x_chip_mode
{
    SX126X_CHIP_MODE_UNUSED = 0x00,
    SX126X_CHIP_MODE_RFU = 0x01,
    SX126X_CHIP_MODE_STBY_RC = 0x02,
    SX126X_CHIP_MODE_STBY_XOSC = 0x03,
    SX126X_CHIP_MODE_FS = 0x04,
    SX126X_CHIP_MODE_RX = 0x05,
    SX126X_CHIP_MODE_TX = 0x06,
} sx126x_chip_mode_t;

/**
 * @brief SX126X Command Status
 */
typedef enum sx126x_command_status
{
    SX126X_COMMAND_STATUS_RESERVED = 0x00,
    SX126X_COMMAND_STATUS_RFU = 0x01,
    SX126X_COMMAND_STATUS_DATA_AVAILABLE = 0x02,
    SX126X_COMMAND_STATUS_CMD_TIMEOUT = 0x03,
    SX126X_COMMAND_STATUS_CMD_PROCESSING_ERROR = 0x04,
    SX126X_COMMAND_STATUS_CMD_EXEC_FAIL = 0x05,
    SX126X_COMMAND_STATUS_CMD_TX_DONE = 0x06,
} sx126x_command_status_t;

/**
 * @brief Initializes the radio
 *
 * @param data The radio configuration
 * @param pinout The radio pinout
 */
void sx126x_init(sx126x_config_t *data, sx126x_pinout_t *pinout);

/**
 * @brief Puts radio to sleep
 *
 * @param data The radio configuration
 */
void sx126x_set_sleep(sx126x_config_t *data, bool coldStart);

/**
 * @brief Puts radio to standby
 *
 * @param data The radio configuration
 * @param mode The standby mode
 */
void sx126x_set_standby(sx126x_config_t *data, sx126x_standby_mode_t mode);

/**
 * @brief Sets radio to FS mode
 *
 * @param data The radio configuration
 */
void sx126x_set_fs(sx126x_config_t *data);

/**
 * @brief Sets radio to TX mode
 *
 * @param data The radio configuration
 * @param timeout_ms The timeout in milliseconds
 */
void sx126x_set_tx(sx126x_config_t *data, unsigned int timeout_ms);

/**
 * @brief Sets radio to RX mode
 *
 * @param data The radio configuration
 * @param timeout_ms The timeout in milliseconds
 */
void sx126x_set_rx(sx126x_config_t *data, unsigned int timeout_ms);

/**
 * @brief Stops radio timer on preamble
 *
 * @param data The radio configuration
 * @param enable Enable or disable
 */
void sx126x_stop_timer_on_preamble(sx126x_config_t *data, bool enable);

/**
 * @brief Set RX duty cycle
 *
 * @param data The radio configuration
 * @param rxPeriod_ms The RX period in milliseconds
 * @param sleepPeriod_ms The sleep period in milliseconds
 */
void sx126x_set_rx_duty_cycle(sx126x_config_t *data, unsigned int rxPeriod_ms, unsigned int sleepPeriod_ms);

/**
 * @brief Sets radio to CAD mode
 *
 * @param data The radio configuration
 */
void sx126x_set_cad(sx126x_config_t *data);

/**
 * @brief Sets radio to TX continuous wave mode
 *
 * @param data The radio configuration
 */
void sx126x_set_tx_continuous_wave(sx126x_config_t *data);

/**
 * @brief Sets radio to TX infinite preamble mode
 *
 * @param data The radio configuration
 */
void sx126x_set_tx_infinite_preamble(sx126x_config_t *data);

/**
 * @brief Sets radio to RX infinite mode
 *
 * @param data The radio configuration
 * @param mode The regulator mode
 */
void sx126x_set_regulator_mode(sx126x_config_t *data, sx126x_regulator_mode_t mode);

/**
 * @brief Calibrates the radio
 *
 * @param data The radio configuration
 * @param calibParam The calibration parameter
 */
void sx126x_calibrate(sx126x_config_t *data, uint8_t calibParam);

/**
 * @brief Calibrates the radio image
 *
 * @param data The radio configuration
 * @param freq_hz The frequency in hertz
 */
void sx126x_calibrate_image(sx126x_config_t *data, unsigned int freq_hz);

/**
 * @brief Sets the PA config
 *
 * @param data The radio configuration
 * @param paDutyCycle The PA duty cycle
 * @param hpMax The HP max
 */
void sx126x_set_pa_config(sx126x_config_t *data, uint8_t paDutyCycle, uint8_t hpMax);

/**
 * @brief Sets the RX TX fallback mode
 *
 * @param data The radio configuration
 * @param fallbackMode The fallback mode
 */
void sx126x_set_rx_tx_fallback_mode(sx126x_config_t *data, sx126x_fallback_mode_t fallbackMode);

/**
 * @brief Writes data to the radio buffer
 *
 * @param data The radio configuration
 * @param offset The offset
 * @param buffer The buffer
 * @param szBuffer The buffer size
 */
void sx126x_write_buffer(sx126x_config_t *data, uint8_t offset, uint8_t *buffer, size_t szBuffer);

/**
 * @brief Reads data from the radio buffer
 *
 * @param data The radio configuration
 * @param offset The offset
 * @param buffer The buffer
 * @param szBuffer The buffer size
 */
void sx126x_read_buffer(sx126x_config_t *data, uint8_t offset, uint8_t *buffer, size_t szBuffer);

/**
 * @brief Sets the DIO IRQ params
 *
 * @param data The radio configuration
 * @param irqMask The IRQ mask
 * @param dio1Mask The DIO1 mask
 * @param dio2Mask The DIO2 mask
 * @param dio3Mask The DIO3 mask
 */
void sx126x_set_dio_irq_params(sx126x_config_t *data, uint16_t irqMask, uint16_t dio1Mask, uint16_t dio2Mask, uint16_t dio3Mask);

/**
 * @brief Gets the IRQ status
 *
 * @param data The radio configuration
 * @param irqStatus The IRQ status
 */
void sx126x_get_irq_status(sx126x_config_t *data, uint16_t *irqStatus);

/**
 * @brief Clears the IRQ status
 *
 * @param data The radio configuration
 * @param clearIrqParam The IRQ param
 */
void sx126x_clear_irq_status(sx126x_config_t *data, uint16_t clearIrqParam);

/**
 * @brief Sets the DIO2 as RF switch control
 *
 * @param data The radio configuration
 * @param enable Enable or disable
 */
void sx126x_set_dio2_as_rf_switch_ctrl(sx126x_config_t *data, bool enable);

/**
 * @brief Sets the DIO3 as TCXO control
 *
 * @param data The radio configuration
 * @param voltage The TCXO voltage
 * @param timeout_ms The timeout in milliseconds
 */
void sx126x_set_dio3_as_tcxo_ctrl(sx126x_config_t *data, sx126x_tcxo_voltage_t voltage, unsigned int timeout_ms);

/**
 * @brief Sets the RF frequency
 *
 * @param data The radio configuration
 * @param frequency The frequency in hertz
 */
void sx126x_set_rf_frequency(sx126x_config_t *data, unsigned int frequency);

/**
 * @brief Sets the packet type
 *
 * @param data The radio configuration
 * @param packetType The packet type
 */
void sx126x_set_packet_type(sx126x_config_t *data, sx126x_packet_type_t packetType);

/**
 * @brief Gets current packet type
 *
 * @param data The radio configuration
 * @param packetType The packet type
 */
void sx126x_get_packet_type(sx126x_config_t *data, sx126x_packet_type_t *packetType);

/**
 * @brief Sets the TX params
 *
 * @param data The radio configuration
 * @param power The power
 * @param rampTime The ramp time
 */
void sx126x_set_tx_params(sx126x_config_t *data, int8_t power, sx126x_ramp_time_t rampTime);

/**
 * @brief Sets the modulation params for GFSK
 *
 * @param data The radio configuration
 * @param bitrate The bitrate
 * @param pulseShape The pulse shape
 * @param bandwidth The bandwidth
 * @param freq_dev_hz The frequency deviation in hertz
 */
void sx126x_set_gfsk_modulation_params(sx126x_config_t *data, unsigned int bitrate, sx126x_gfsk_pulse_shape_t pulseShape, sx126x_gfsk_bandwidth_t bandwidth, unsigned int freq_dev_hz);

/**
 * @brief Sets the modulation params for LoRa
 *
 * @param data The radio configuration
 * @param sf The spreading factor
 * @param bw The bandwidth
 * @param cr The coding rate
 * @param lowDataRateOptimize Enable or disable low data rate optimization
 */
void sx126x_set_lora_modulation_params(sx126x_config_t *data, sx126x_lora_sf_t sf, sx126x_lora_bw_t bw, sx126x_lora_cr_t cr, bool lowDataRateOptimize);

/**
 * @brief Sets the packet params for GFSK
 *
 * @param data The radio configuration
 * @param preambleLength The preamble length
 * @param detectorLength The preamble detector length
 * @param syncWordLength The sync word length
 * @param addressFiltering The address filtering
 * @param packetType The packet type
 * @param payloadLength The payload length
 * @param crcType The CRC type
 * @param whitening Enable or disable whitening
 */
void sx126x_set_packet_gfsk_params(sx126x_config_t *data, uint16_t preambleLength, sx126x_gfsk_preamble_detector_length_t detectorLength, uint8_t syncWordLength, sx126x_gfsk_address_filtering_t addressFiltering, sx126x_gfsk_packet_type_t packetType, uint8_t payloadLength, sx126x_gfsk_crc_type_t crcType, bool whitening);

/**
 * @brief Sets the packet params for LoRa
 *
 * @param data The radio configuration
 * @param preambleLength The preamble length
 * @param headerType The header type
 * @param payloadLength The payload length
 * @param crc Enable or disable CRC
 * @param invertIQ Enable or disable IQ inversion
 */
void sx126x_set_packet_lora_params(sx126x_config_t *data, uint16_t preambleLength, sx126x_lora_header_type_t headerType, uint8_t payloadLength, bool crc, bool invertIQ);

/**
 * @brief Sets the CAD params
 *
 * @param data The radio configuration
 * @param cadSymbolNum The CAD symbol number
 * @param cadDetPeak The CAD detection peak
 * @param cadDetMin The CAD detection minimum
 * @param cadExitMode The CAD exit mode
 * @param timeout_ms The timeout in milliseconds
 */
void sx126x_set_cad_params(sx126x_config_t *data, sx126x_cad_symbol_t cadSymbolNum, uint8_t cadDetPeak, uint8_t cadDetMin, sx126x_lora_cad_exit_mode_t cadExitMode, unsigned int timeout_ms);

/**
 * @brief Sets the buffer base address
 *
 * @param data The radio configuration
 * @param txBaseAddress The TX base address
 * @param rxBaseAddress The RX base address
 */
void sx126x_set_buffer_base_address(sx126x_config_t *data, uint8_t txBaseAddress, uint8_t rxBaseAddress);

/**
 * @brief Sets the LoRa symbols number timeout
 *
 * @param data The radio configuration
 * @param symbNum The symbols number
 */
void sx126x_set_lora_symb_num_timeout(sx126x_config_t *data, uint8_t symbNum);

/**
 * @brief Gets the status of the radio
 *
 * @param data The radio configuration
 * @param pChipMode The chip mode
 * @param pCmdStatus The command status
 */
void sx126x_get_status(sx126x_config_t *data, sx126x_chip_mode_t *pChipMode, sx126x_command_status_t *pCmdStatus);

/**
 * @brief Gets the RX buffer status
 *
 * @param data The radio configuration
 * @param pPayloadLength The payload length
 * @param pRXStartBufferPtr The RX start buffer pointer
 */
void sx126x_get_rx_buffer_status(sx126x_config_t *data, uint8_t *pPayloadLength, uint8_t *pRXStartBufferPtr);

/**
 * @brief Gets the GFSK packet status
 *
 * @param data The radio configuration
 * @param pRXStatus The RX status
 * @param pRssiSync The RSSI sync
 * @param pRssiAvg The RSSI average
 */
void sx126x_get_gfsk_packet_status(sx126x_config_t *data, sx126x_gfsk_packet_rx_status_t *pRXStatus, int8_t *pRssiSync, int8_t *pRssiAvg);

/**
 * @brief Gets the LoRa packet status
 *
 * @param data The radio configuration
 * @param pRssiPacket The RSSI packet
 * @param pSnrPkt The SNR packet
 * @param pSignalRssiPkt The signal RSSI packet
 */
void sx126x_get_lora_packet_status(sx126x_config_t *data, int8_t *pRssiPacket, int8_t *pSnrPkt, int8_t *pSignalRssiPkt);

/**
 * @brief Gets the RSSI inst
 *
 * @param data The radio configuration
 * @param pRssiInst The RSSI inst
 */
void sx126x_get_rssi_inst(sx126x_config_t *data, int8_t *pRssiInst);

/**
 * @brief Gets the stats for GFSK
 *
 * @param data The radio configuration
 * @param pNbPktReceived The number of packets received
 * @param pNbPktCrcErr The number of packets with CRC error
 * @param pNbPktLenErr The number of packets with length error
 */
void sx126x_get_gfsk_stats(sx126x_config_t *data, uint16_t *pNbPktReceived, uint16_t *pNbPktCrcErr, uint16_t *pNbPktLenErr);

/**
 * @brief Gets the stats for LoRa
 *
 * @param data The radio configuration
 * @param pNbPktReceived The number of packets received
 * @param pNbPktCrcErr The number of packets with CRC error
 * @param pNbPktHeaderErr The number of packets with header error
 */
void sx126x_get_lora_stats(sx126x_config_t *data, uint16_t *pNbPktReceived, uint16_t *pNbPktCrcErr, uint16_t *pNbPktHeaderErr);

/**
 * @brief Resets the stats
 *
 * @param data The radio configuration
 */
void sx126x_reset_stats(sx126x_config_t *data);

/**
 * @brief Gets the device errors
 *
 * @param data The radio configuration
 * @param pErrors The errors
 */
void sx126x_get_device_errors(sx126x_config_t *data, uint16_t *pErrors);

/**
 * @brief Clears the device errors
 *
 * @param data The radio configuration
 */
void sx126x_clear_device_errors(sx126x_config_t *data);

/**
 * @brief Checks if the radio is busy
 *
 * @param data The radio configuration
 * @param pStatus The status
 */
void sx126x_is_busy(sx126x_config_t *data, bool *pStatus);

/**
 * @brief Checks if the radio is busy and waits
 *
 * @param data The radio configuration
 */
void sx126x_check_busy(sx126x_config_t *data);

/**
 * @brief Resets the radio
 *
 * @param data The radio configuration
 */
void sx126x_reset(sx126x_config_t *data);

/**
 * @brief Wakes up the radio
 *
 * @param data The radio configuration
 */
void sx126x_wakeup(sx126x_config_t *data);

/**
 * @brief Clamps the TX power
 *
 * @param data The radio configuration
 */
void sx126x_clamp_tx(sx126x_config_t *data);

/**
 * @brief Stops the RTC
 *
 * @param data The radio configuration
 */
void sx126x_stop_rtc(sx126x_config_t *data);

#endif