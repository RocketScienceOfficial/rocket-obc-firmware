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
    PinNumber busy;  /** BUSY */
} SX126XPinout;

/**
 * @brief LoRa radio configuration. All changes are done internally, so keep it just for reference!
 */
typedef struct SX126XConfig
{
    SX126XPinout pinout; /** Pinout */
} SX126XConfig;

/**
 * @brief SX126X GFSK packet RX status structure
 */
typedef struct SX126XGFSKPacketRXStatus
{
    BOOL preambleErr;
    BOOL syncErr;
    BOOL addrErr;
    BOOL crcErr;
    BOOL lengthErr;
    BOOL abortErr;
    BOOL packetReceived;
    BOOL packetSent;
} SX126XGFSKPacketRXStatus;

/**
 * @brief SX126X Standy mode
 */
typedef enum SX126XStandbyMode
{
    SX126X_STANDBY_RC = 0x00,
    SX126X_STANDBY_XOSC = 0x01,
} SX126XStandbyMode;

/**
 * @brief SX126X Regulator mode
 */
typedef enum SX126XRegulatorMode
{
    SX126X_REGULATOR_LDO = 0x00,
    SX126X_REGULATOR_DC_DC = 0x01,
} SX126XRegulatorMode;

/**
 * @brief SX126X Fallback mode
 */
typedef enum SX126XFallbackMode
{
    SX126X_FALLBACK_MODE_STDBY_RC = 0x20,
    SX126X_FALLBACK_MODE_STDBY_XOSC = 0x30,
    SX126X_FALLBACK_MODE_FS = 0x40,
} SX126XFallbackMode;

/**
 * @brief SX126X IRQ mask
 */
typedef enum SX126XIRQMask
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
} SX126XIRQMask;

/**
 * @brief SX126X Calibration Mask
 */
typedef enum SX126XCalibrationMask
{
    SX126X_CALIBRATE_RC64K_MASK = (1 << 0),
    SX126X_CALIBRATE_RC13M_MASK = (1 << 1),
    SX126X_CALIBRATE_PLL_MASK = (1 << 2),
    SX126X_CALIBRATE_ADC_PULSE_MASK = (1 << 3),
    SX126X_CALIBRATE_ADC_BULK_N_MASK = (1 << 4),
    SX126X_CALIBRATE_ADC_BULK_P_MASK = (1 << 5),
    SX126X_CALIBRATE_IMAGE_MASK = (1 << 6),
    SX126X_CALIBRATE_ALL_MASK = SX126X_CALIBRATE_RC64K_MASK | SX126X_CALIBRATE_RC13M_MASK | SX126X_CALIBRATE_PLL_MASK | SX126X_CALIBRATE_ADC_PULSE_MASK | SX126X_CALIBRATE_ADC_BULK_N_MASK | SX126X_CALIBRATE_ADC_BULK_P_MASK | SX126X_CALIBRATE_IMAGE_MASK,
} SX126XCalibrationMask;

/**
 * @brief SX126X TCXO voltage
 */
typedef enum SX126XTCXOVoltage
{
    SX126X_TCXO_CTRL_1_6V = 0x00,
    SX126X_TCXO_CTRL_1_7V = 0x01,
    SX126X_TCXO_CTRL_1_8V = 0x02,
    SX126X_TCXO_CTRL_2_2V = 0x03,
    SX126X_TCXO_CTRL_2_4V = 0x04,
    SX126X_TCXO_CTRL_2_7V = 0x05,
    SX126X_TCXO_CTRL_3_0V = 0x06,
    SX126X_TCXO_CTRL_3_3V = 0x07,
} SX126XTCXOVoltage;

/**
 * @brief SX126X Ramp Time
 */
typedef enum SX126XRampTime
{
    SX126X_RAMP_10U = 0x00,
    SX126X_RAMP_20U = 0x01,
    SX126X_RAMP_40U = 0x02,
    SX126X_RAMP_80U = 0x03,
    SX126X_RAMP_200U = 0x04,
    SX126X_RAMP_800U = 0x05,
    SX126X_RAMP_1700U = 0x06,
    SX126X_RAMP_3400U = 0x07,
} SX126XRampTime;

/**
 * @brief SX126X Packet Type
 */
typedef enum SX126XPacketType
{
    SX126X_PACKET_TYPE_GFSK = 0x00,
    SX126X_PACKET_TYPE_LORA = 0x01,
} SX126XPacketType;

/**
 * @brief SX126X GFSK Pulse Shape
 */
typedef enum SX126XGFSKPulseShape
{
    SX126X_GFSK_PULSE_SHAPE_OFF = 0x00,
    SX126X_GFSK_PULSE_SHAPE_BT_03 = 0x08,
    SX126X_GFSK_PULSE_SHAPE_BT_05 = 0x09,
    SX126X_GFSK_PULSE_SHAPE_BT_07 = 0x0A,
    SX126X_GFSK_PULSE_SHAPE_BT_1 = 0x0B,
} SX126XGFSKPulseShape;

/**
 * @brief SX126X GFSK Bandwidth
 */
typedef enum SX126XGFSKBandwidth
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
} SX126XGFSKBandwidth;

/**
 * @brief SX126X LoRa Spreading Factor
 */
typedef enum SX126XLoRaSF
{
    SX126X_LORA_SF_5 = 0x05,
    SX126X_LORA_SF_6 = 0x06,
    SX126X_LORA_SF_7 = 0x07,
    SX126X_LORA_SF_8 = 0x08,
    SX126X_LORA_SF_9 = 0x09,
    SX126X_LORA_SF_10 = 0x0A,
    SX126X_LORA_SF_11 = 0x0B,
    SX126X_LORA_SF_12 = 0x0C,
} SX126XLoRaSF;

/**
 * @brief SX126X LoRa Bandwidth
 */
typedef enum SX126XLoRaBW
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
} SX126XLoRaBW;

/**
 * @brief SX126X LoRa Coding Rate
 */
typedef enum SX126XLoRaCR
{
    SX126X_LORA_CR_4_5 = 0x01,
    SX126X_LORA_CR_4_6 = 0x02,
    SX126X_LORA_CR_4_7 = 0x03,
    SX126X_LORA_CR_4_8 = 0x04,
} SX126XLoRaCR;

/**
 * @brief SX126X GFSK Preamble Detector Length
 */
typedef enum SX126XGFSKPreambleDetectorLength
{
    SX126X_GFSK_PREAMBLE_DETECTOR_LENGTH_OFF = 0x00,
    SX126X_GFSK_PREAMBLE_DETECTOR_LENGTH_08B = 0x04,
    SX126X_GFSK_PREAMBLE_DETECTOR_LENGTH_16B = 0x05,
    SX126X_GFSK_PREAMBLE_DETECTOR_LENGTH_24B = 0x06,
    SX126X_GFSK_PREAMBLE_DETECTOR_LENGTH_32B = 0x07,
} SX126XGFSKPreambleDetectorLength;

/**
 * @brief SX126X GFSK Address Filtering
 */
typedef enum SX126XGFSKAddressFiltering
{
    SX126X_GFSK_ADDRESS_FILTERING_OFF = 0x00,
    SX126X_GFSK_ADDRESS_FILTERING_NODE = 0x01,
    SX126X_GFSK_ADDRESS_FILTERING_NODE_BROADCAST = 0x02,
} SX126XGFSKAddressFiltering;

/**
 * @brief SX126X GFSK Packet Type
 */
typedef enum SX126XGFSKPacketType
{
    SX126X_GFSK_PACKET_TYPE_VAR_LEN = 0x00,
    SX126X_GFSK_PACKET_TYPE_FIX_LEN = 0x01,
} SX126XGFSKPacketType;

/**
 * @brief SX126X GFSK CRC Type
 */
typedef enum SX126XGFSKCRCType
{
    SX126X_GFSK_CRC_OFF = 0x01,
    SX126X_GFSK_CRC_1_BYTE = 0x00,
    SX126X_GFSK_CRC_2_BYTE = 0x02,
    SX126X_GFSK_CRC_1_BYTE_INV = 0x04,
    SX126X_GFSK_CRC_2_BYTE_INV = 0x06,
} SX126XGFSKCRCType;

/**
 * @brief SX126X LoRa Header Type
 */
typedef enum SX126XLoRaHeaderType
{
    SX126X_LORA_HEADER_IMPLICIT = 0x00,
    SX126X_LORA_HEADER_EXPLICIT = 0x01,
} SX126XLoRaHeaderType;

/**
 * @brief SX126X CAD Symbol
 */
typedef enum SX126XCADSymbol
{
    SX126X_CAD_SYMBOL_1 = 0x00,
    SX126X_CAD_SYMBOL_2 = 0x01,
    SX126X_CAD_SYMBOL_4 = 0x02,
    SX126X_CAD_SYMBOL_8 = 0x03,
    SX126X_CAD_SYMBOL_16 = 0x04,
} SX126XCADSymbol;

/**
 * @brief SX126X CAD Exit Mode
 */
typedef enum SX126XCADExitMode
{
    SX126X_CAD_ONLY = 0x00,
    SX126X_CAD_RX = 0x01,
} SX126XLoRaCADExitMode;

/**
 * @brief SX126X Chip Mode
 */
typedef enum SX126XChipMode
{
    SX126X_CHIP_MODE_UNUSED = 0x00,
    SX126X_CHIP_MODE_RFU = 0x01,
    SX126X_CHIP_MODE_STBY_RC = 0x02,
    SX126X_CHIP_MODE_STBY_XOSC = 0x03,
    SX126X_CHIP_MODE_FS = 0x04,
    SX126X_CHIP_MODE_RX = 0x05,
    SX126X_CHIP_MODE_TX = 0x06,
} SX126XChipMode;

/**
 * @brief SX126X Command Status
 */
typedef enum SX126XCommandStatus
{
    SX126X_COMMAND_STATUS_RESERVED = 0x00,
    SX126X_COMMAND_STATUS_RFU = 0x01,
    SX126X_COMMAND_STATUS_DATA_AVAILABLE = 0x02,
    SX126X_COMMAND_STATUS_CMD_TIMEOUT = 0x03,
    SX126X_COMMAND_STATUS_CMD_PROCESSING_ERROR = 0x04,
    SX126X_COMMAND_STATUS_CMD_EXEC_FAIL = 0x05,
    SX126X_COMMAND_STATUS_CMD_TX_DONE = 0x06,
} SX126XCommandStatus;

/**
 * @brief Initializes the radio
 *
 * @param data The radio configuration
 * @param pinout The radio pinout
 * @return Result code
 */
FUNCRESULT sx126XInit(SX126XConfig *data, SX126XPinout *pinout);

/**
 * @brief Puts radio to sleep
 *
 * @param data The radio configuration
 * @return Result code
 */
FUNCRESULT sx126XSetSleep(SX126XConfig *data, BOOL coldStart);

/**
 * @brief Puts radio to standby
 *
 * @param data The radio configuration
 * @param mode The standby mode
 * @return Result code
 */
FUNCRESULT sx126XSetStandby(SX126XConfig *data, SX126XStandbyMode mode);

/**
 * @brief Sets radio to FS mode
 *
 * @param data The radio configuration
 * @return Result code
 */
FUNCRESULT sx126XSetFS(SX126XConfig *data);

/**
 * @brief Sets radio to TX mode
 *
 * @param data The radio configuration
 * @param timeout_ms The timeout in milliseconds
 * @return Result code
 */
FUNCRESULT sx126XSetTX(SX126XConfig *data, UINT32 timeout_ms);

/**
 * @brief Sets radio to RX mode
 *
 * @param data The radio configuration
 * @param timeout_ms The timeout in milliseconds
 * @return Result code
 */
FUNCRESULT sx126XSetRX(SX126XConfig *data, UINT32 timeout_ms);

/**
 * @brief Stops radio timer on preamble
 *
 * @param data The radio configuration
 * @param enable Enable or disable
 * @return Result code
 */
FUNCRESULT sx126XStopTimerOnPreamble(SX126XConfig *data, BOOL enable);

/**
 * @brief Set RX duty cycle
 *
 * @param data The radio configuration
 * @param rxPeriod_ms The RX period in milliseconds
 * @param sleepPeriod_ms The sleep period in milliseconds
 * @return Result code
 */
FUNCRESULT sx126XSetRXDutyCycle(SX126XConfig *data, UINT32 rxPeriod_ms, UINT32 sleepPeriod_ms);

/**
 * @brief Sets radio to CAD mode
 *
 * @param data The radio configuration
 * @return Result code
 */
FUNCRESULT sx126XSetCAD(SX126XConfig *data);

/**
 * @brief Sets radio to TX continuous wave mode
 *
 * @param data The radio configuration
 * @return Result code
 */
FUNCRESULT sx126XSetTXContinuousWave(SX126XConfig *data);

/**
 * @brief Sets radio to TX infinite preamble mode
 *
 * @param data The radio configuration
 * @return Result code
 */
FUNCRESULT sx126XSetTXInfinitePreamble(SX126XConfig *data);

/**
 * @brief Sets radio to RX infinite mode
 *
 * @param data The radio configuration
 * @param mode The regulator mode
 * @return Result code
 */
FUNCRESULT sx126XSetRegulatorMode(SX126XConfig *data, SX126XRegulatorMode mode);

/**
 * @brief Calibrates the radio
 *
 * @param data The radio configuration
 * @param calibParam The calibration parameter
 * @return Result code
 */
FUNCRESULT sx126XCalibrate(SX126XConfig *data, UINT8 calibParam);

/**
 * @brief Calibrates the radio image
 *
 * @param data The radio configuration
 * @param freq_hz The frequency in hertz
 * @return Result code
 */
FUNCRESULT sx126XCalibrateImage(SX126XConfig *data, UINT32 freq_hz);

/**
 * @brief Sets the PA config
 *
 * @param data The radio configuration
 * @param paDutyCycle The PA duty cycle
 * @param hpMax The HP max
 * @return Result code
 */
FUNCRESULT sx126XSetPAConfig(SX126XConfig *data, UINT8 paDutyCycle, UINT8 hpMax);

/**
 * @brief Sets the RX TX fallback mode
 *
 * @param data The radio configuration
 * @param fallbackMode The fallback mode
 * @return Result code
 */
FUNCRESULT sx126XSetRXTXFallbackMode(SX126XConfig *data, SX126XFallbackMode fallbackMode);

/**
 * @brief Writes data to the radio buffer
 *
 * @param data The radio configuration
 * @param offset The offset
 * @param buffer The buffer
 * @param szBuffer The buffer size
 * @return Result code
 */
FUNCRESULT sx126XWriteBuffer(SX126XConfig *data, UINT8 offset, BYTE *buffer, SIZE szBuffer);

/**
 * @brief Reads data from the radio buffer
 *
 * @param data The radio configuration
 * @param offset The offset
 * @param buffer The buffer
 * @param szBuffer The buffer size
 * @return Result code
 */
FUNCRESULT sx126XReadBuffer(SX126XConfig *data, UINT8 offset, BYTE *buffer, SIZE szBuffer);

/**
 * @brief Sets the DIO IRQ params
 *
 * @param data The radio configuration
 * @param irqMask The IRQ mask
 * @param dio1Mask The DIO1 mask
 * @param dio2Mask The DIO2 mask
 * @param dio3Mask The DIO3 mask
 * @return Result code
 */
FUNCRESULT sx126XSetDIOIRQParams(SX126XConfig *data, UINT16 irqMask, UINT16 dio1Mask, UINT16 dio2Mask, UINT16 dio3Mask);

/**
 * @brief Gets the IRQ status
 *
 * @param data The radio configuration
 * @param irqStatus The IRQ status
 * @return Result code
 */
FUNCRESULT sx126XGetIRQStatus(SX126XConfig *data, UINT16 *irqStatus);

/**
 * @brief Clears the IRQ status
 *
 * @param data The radio configuration
 * @param clearIrqParam The IRQ param
 * @return Result code
 */
FUNCRESULT sx126XClearIRQStatus(SX126XConfig *data, UINT16 clearIrqParam);

/**
 * @brief Sets the DIO2 as RF switch control
 *
 * @param data The radio configuration
 * @param enable Enable or disable
 * @return Result code
 */
FUNCRESULT sx126XSetDIO2AsRFSwitchCtrl(SX126XConfig *data, BOOL enable);

/**
 * @brief Sets the DIO3 as TCXO control
 *
 * @param data The radio configuration
 * @param voltage The TCXO voltage
 * @param timeout_ms The timeout in milliseconds
 * @return Result code
 */
FUNCRESULT sx126XSetDIO3AsTCXOCtrl(SX126XConfig *data, SX126XTCXOVoltage voltage, UINT32 timeout_ms);

/**
 * @brief Sets the RF frequency
 *
 * @param data The radio configuration
 * @param frequency The frequency in hertz
 * @return Result code
 */
FUNCRESULT sx126XSetRFFrequency(SX126XConfig *data, UINT32 frequency);

/**
 * @brief Sets the packet type
 *
 * @param data The radio configuration
 * @param packetType The packet type
 * @return Result code
 */
FUNCRESULT sx126XSetPacketType(SX126XConfig *data, SX126XPacketType packetType);

/**
 * @brief Gets current packet type
 *
 * @param data The radio configuration
 * @param packetType The packet type
 * @return Result code
 */
FUNCRESULT sx126XGetPacketType(SX126XConfig *data, SX126XPacketType *packetType);

/**
 * @brief Sets the TX params
 *
 * @param data The radio configuration
 * @param power The power
 * @param rampTime The ramp time
 * @return Result code
 */
FUNCRESULT sx126XSetTXParams(SX126XConfig *data, INT8 power, SX126XRampTime rampTime);

/**
 * @brief Sets the modulation params for GFSK
 *
 * @param data The radio configuration
 * @param bitrate The bitrate
 * @param pulseShape The pulse shape
 * @param bandwidth The bandwidth
 * @param freq_dev_hz The frequency deviation in hertz
 * @return Result code
 */
FUNCRESULT sx126XSetGFSKModulationParams(SX126XConfig *data, UINT32 bitrate, SX126XGFSKPulseShape pulseShape, SX126XGFSKBandwidth bandwidth, UINT32 freq_dev_hz);

/**
 * @brief Sets the modulation params for LoRa
 *
 * @param data The radio configuration
 * @param sf The spreading factor
 * @param bw The bandwidth
 * @param cr The coding rate
 * @param lowDataRateOptimize Enable or disable low data rate optimization
 * @return Result code
 */
FUNCRESULT sx126XSetLoRaModulationParams(SX126XConfig *data, SX126XLoRaSF sf, SX126XLoRaBW bw, SX126XLoRaCR cr, BOOL lowDataRateOptimize);

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
 * @return Result code
 */
FUNCRESULT sx126XSetPacketGFSKParams(SX126XConfig *data, UINT16 preambleLength, SX126XGFSKPreambleDetectorLength detectorLength, UINT8 syncWordLength, SX126XGFSKAddressFiltering addressFiltering, SX126XGFSKPacketType packetType, UINT8 payloadLength, SX126XGFSKCRCType crcType, BOOL whitening);

/**
 * @brief Sets the packet params for LoRa
 *
 * @param data The radio configuration
 * @param preambleLength The preamble length
 * @param headerType The header type
 * @param payloadLength The payload length
 * @param crc Enable or disable CRC
 * @param invertIQ Enable or disable IQ inversion
 * @return Result code
 */
FUNCRESULT sx126XSetPacketLoRaParams(SX126XConfig *data, UINT16 preambleLength, SX126XLoRaHeaderType headerType, UINT8 payloadLength, BOOL crc, BOOL invertIQ);

/**
 * @brief Sets the CAD params
 *
 * @param data The radio configuration
 * @param cadSymbolNum The CAD symbol number
 * @param cadDetPeak The CAD detection peak
 * @param cadDetMin The CAD detection minimum
 * @param cadExitMode The CAD exit mode
 * @param timeout_ms The timeout in milliseconds
 * @return Result code
 */
FUNCRESULT sx126XSetCADParams(SX126XConfig *data, SX126XCADSymbol cadSymbolNum, UINT8 cadDetPeak, UINT8 cadDetMin, SX126XLoRaCADExitMode cadExitMode, UINT32 timeout_ms);

/**
 * @brief Sets the buffer base address
 *
 * @param data The radio configuration
 * @param txBaseAddress The TX base address
 * @param rxBaseAddress The RX base address
 * @return Result code
 */
FUNCRESULT sx126XSetBufferBaseAddress(SX126XConfig *data, UINT8 txBaseAddress, UINT8 rxBaseAddress);

/**
 * @brief Sets the LoRa symbols number timeout
 *
 * @param data The radio configuration
 * @param symbNum The symbols number
 * @return Result code
 */
FUNCRESULT sx126XSetLoRaSymbNumTimeout(SX126XConfig *data, UINT8 symbNum);

/**
 * @brief Gets the status of the radio
 *
 * @param data The radio configuration
 * @param pChipMode The chip mode
 * @param pCmdStatus The command status
 * @return Result code
 */
FUNCRESULT sx126XGetStatus(SX126XConfig *data, SX126XChipMode *pChipMode, SX126XCommandStatus *pCmdStatus);

/**
 * @brief Gets the RX buffer status
 *
 * @param data The radio configuration
 * @param pPayloadLength The payload length
 * @param pRXStartBufferPtr The RX start buffer pointer
 * @return Result code
 */
FUNCRESULT sx126XGetRXBufferStatus(SX126XConfig *data, UINT8 *pPayloadLength, UINT8 *pRXStartBufferPtr);

/**
 * @brief Gets the GFSK packet status
 *
 * @param data The radio configuration
 * @param pRXStatus The RX status
 * @param pRssiSync The RSSI sync
 * @param pRssiAvg The RSSI average
 * @return Result code
 */
FUNCRESULT sx126XGetGFSKPacketStatus(SX126XConfig *data, SX126XGFSKPacketRXStatus *pRXStatus, INT8 *pRssiSync, INT8 *pRssiAvg);

/**
 * @brief Gets the LoRa packet status
 *
 * @param data The radio configuration
 * @param pRssiPacket The RSSI packet
 * @param pSnrPkt The SNR packet
 * @param pSignalRssiPkt The signal RSSI packet
 * @return Result code
 */
FUNCRESULT sx126XGetLoRaPacketStatus(SX126XConfig *data, INT8 *pRssiPacket, INT8 *pSnrPkt, INT8 *pSignalRssiPkt);

/**
 * @brief Gets the RSSI inst
 *
 * @param data The radio configuration
 * @param pRssiInst The RSSI inst
 * @return Result code
 */
FUNCRESULT sx126XGetRssiInst(SX126XConfig *data, INT8 *pRssiInst);

/**
 * @brief Gets the stats for GFSK
 *
 * @param data The radio configuration
 * @param pNbPktReceived The number of packets received
 * @param pNbPktCrcErr The number of packets with CRC error
 * @param pNbPktLenErr The number of packets with length error
 * @return Result code
 */
FUNCRESULT sx126XGetGFSKStats(SX126XConfig *data, UINT16 *pNbPktReceived, UINT16 *pNbPktCrcErr, UINT16 *pNbPktLenErr);

/**
 * @brief Gets the stats for LoRa
 *
 * @param data The radio configuration
 * @param pNbPktReceived The number of packets received
 * @param pNbPktCrcErr The number of packets with CRC error
 * @param pNbPktHeaderErr The number of packets with header error
 * @return Result code
 */
FUNCRESULT sx126XGetLoRaStats(SX126XConfig *data, UINT16 *pNbPktReceived, UINT16 *pNbPktCrcErr, UINT16 *pNbPktHeaderErr);

/**
 * @brief Resets the stats
 *
 * @param data The radio configuration
 * @return Result code
 */
FUNCRESULT sx126XResetStats(SX126XConfig *data);

/**
 * @brief Gets the device errors
 *
 * @param data The radio configuration
 * @param pErrors The errors
 * @return Result code
 */
FUNCRESULT sx126XGetDeviceErrors(SX126XConfig *data, UINT16 *pErrors);

/**
 * @brief Clears the device errors
 *
 * @param data The radio configuration
 * @return Result code
 */
FUNCRESULT sx126XClearDeviceErrors(SX126XConfig *data);

/**
 * @brief Checks if the radio is busy
 *
 * @param data The radio configuration
 * @param pStatus The status
 * @return Result code
 */
FUNCRESULT sx126XIsBusy(SX126XConfig *data, BOOL *pStatus);

/**
 * @brief Checks if the radio is busy and waits
 *
 * @param data The radio configuration
 * @return Result code
 */
FUNCRESULT sx126XCheckBusy(SX126XConfig *data);

/**
 * @brief Resets the radio
 *
 * @param data The radio configuration
 * @return Result code
 */
FUNCRESULT sx126XReset(SX126XConfig *data);

/**
 * @brief Wakes up the radio
 *
 * @param data The radio configuration
 * @return Result code
 */
FUNCRESULT sx126XWakeup(SX126XConfig *data);

/**
 * @brief Clamps the TX power
 *
 * @param data The radio configuration
 * @return Result code
 */
FUNCRESULT sx126XClampTX(SX126XConfig *data);

/**
 * @brief Stops the RTC
 *
 * @param data The radio configuration
 * @return Result code
 */
FUNCRESULT sx126XStopRTC(SX126XConfig *data);

/**
 * @brief Converts timeout is ms to RTC step
 *
 * @param timeout_ms The timeout in ms
 * @return The RTC step
 */
UINT32 __sx126XConvertTimeoutToRTCStep(UINT32 timeout_ms);

/**
 * @brief Converts frequency to register value
 *
 * @param frequency The frequency in Hz
 * @return The register value
 */
UINT32 __sx126XConvertFrequencyToRegisterValue(UINT32 frequency);

/**
 * @brief Does the TX modulation workaround
 *
 * @param data The radio configuration
 * @param packetType The packet type
 * @param bw The bandwidth
 * @return Result code
 */
VOID __sx126XTXModulationWorkaround(SX126XConfig *data, SX126XPacketType packetType, SX126XLoRaBW bw);

/**
 * @brief Writes data to radio register
 *
 * @param data The radio configuration
 * @param address The register address
 * @param buffer The buffer
 * @param szBuffer The buffer size
 */
VOID __sx126XWriteRegister(SX126XConfig *data, UINT16 address, BYTE *buffer, SIZE szBuffer);

/**
 * @brief Reads data from radio register
 *
 * @param data The radio configuration
 * @param address The register address
 * @param buffer The buffer
 * @param szBuffer The buffer size
 */
VOID __sx126XReadRegister(SX126XConfig *data, UINT16 address, BYTE *buffer, SIZE szBuffer);

/**
 * @brief Executes command on radio
 *
 * @param data The radio configuration
 * @param command The command
 * @param params The command parameters
 * @param szParams The command parameters size
 * @param resultBuffer The result buffer
 * @param szBuffer The result buffer size
 * @return Result code
 */
FUNCRESULT __sx126XCMD(SX126XConfig *data, BYTE command, BYTE *params, SIZE szParams, BYTE *resultBuffer, SIZE szBuffer);