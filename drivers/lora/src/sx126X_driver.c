#include "drivers/lora/sx126X_driver.h"
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

FUNCRESULT sx126XInit(SX126XConfig *data, SX126XPinout *pinout, UINT32 frequency)
{
    if (!data || !pinout)
    {
        return ERR_INVALIDARG;
    }

    return SUC_OK;
}

FUNCRESULT sx126XSetSleep(SX126XConfig *data, BOOL coldStart)
{
    BYTE buff[1];

    buff[0] = (BYTE)(coldStart ? 0 : (1 << 2));

    return __sx126XCMD(data, CMD_SET_SLEEP, buff, 1, NULL, 0);
}

FUNCRESULT sx126XSetStandby(SX126XConfig *data, BOOL rc)
{
    BYTE buff[1];

    buff[0] = (BYTE)rc;

    return __sx126XCMD(data, CMD_SET_STANDBY, buff, 1, NULL, 0);
}

FUNCRESULT sx126XSetFS(SX126XConfig *data)
{
    return __sx126XCMD(data, CMD_SET_FS, NULL, 0, NULL, 0);
}

FUNCRESULT sx126XSetTX(SX126XConfig *data, UINT32 timeout_ms)
{
    BYTE buff[3];

    UINT32 timeout = __sx126XConvertTimeoutToRTCStep(timeout_ms);

    buff[0] = (BYTE)(timeout >> 16);
    buff[1] = (BYTE)(timeout >> 8);
    buff[2] = (BYTE)(timeout);

    return __sx126XCMD(data, CMD_SET_TX, buff, 3, NULL, 0);
}

FUNCRESULT sx126XSetRX(SX126XConfig *data, UINT32 timeout_ms)
{
    BYTE buff[3];

    UINT32 timeout = __sx126XConvertTimeoutToRTCStep(timeout_ms);

    buff[0] = (BYTE)(timeout >> 16);
    buff[1] = (BYTE)(timeout >> 8);
    buff[2] = (BYTE)(timeout);

    return __sx126XCMD(data, CMD_SET_RX, buff, 3, NULL, 0);
}

FUNCRESULT sx126XStopTimerOnPreamble(SX126XConfig *data, BOOL enable)
{
    BYTE buff[1];

    buff[0] = (BYTE)enable;

    return __sx126XCMD(data, CMD_STOP_TIMER_ON_PREAMBLE, buff, 1, NULL, 0);
}

FUNCRESULT sx126XSetRXDutyCycle(SX126XConfig *data, UINT32 rxPeriod_ms, UINT32 sleepPeriod_ms)
{
    BYTE buff[6];

    UINT32 rxPeriod = __sx126XConvertTimeoutToRTCStep(rxPeriod_ms);
    UINT32 sleepPeriod = __sx126XConvertTimeoutToRTCStep(sleepPeriod_ms);

    buff[0] = (BYTE)(rxPeriod >> 16);
    buff[1] = (BYTE)(rxPeriod >> 8);
    buff[2] = (BYTE)(rxPeriod);
    buff[3] = (BYTE)(sleepPeriod >> 16);
    buff[4] = (BYTE)(sleepPeriod >> 8);
    buff[5] = (BYTE)(sleepPeriod);

    return __sx126XCMD(data, CMD_SET_RX_DUTY_CYCLE, buff, 6, NULL, 0);
}

FUNCRESULT sx126XSetCAD(SX126XConfig *data)
{
    return __sx126XCMD(data, CMD_SET_CAD, NULL, 0, NULL, 0);
}

FUNCRESULT sx126XSetTXContinuousWave(SX126XConfig *data)
{
    return __sx126XCMD(data, CMD_SET_TX_CONTINUOUS_WAVE, NULL, 0, NULL, 0);
}

FUNCRESULT sx126XSetTXInfinitePreamble(SX126XConfig *data)
{
    return __sx126XCMD(data, CMD_SET_TX_INFINITE_PREAMBLE, NULL, 0, NULL, 0);
}

FUNCRESULT sx126XSetRegulatorMode(SX126XConfig *data, BOOL useDCDC)
{
    BYTE buff[1];

    buff[0] = (BYTE)(useDCDC ? 0 : 1);

    return __sx126XCMD(data, CMD_SET_REGULATOR_MODE, buff, 1, NULL, 0);
}

FUNCRESULT sx126XCalibrate(SX126XConfig *data, UINT8 calibParam)
{
    BYTE buff[1];

    buff[0] = calibParam;

    return __sx126XCMD(data, CMD_CALIBRATE, buff, 1, NULL, 0);
}

FUNCRESULT sx126XCalibrateImage(SX126XConfig *data, UINT32 freq_hz)
{
    BYTE buff[2];

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
        return ERR_INVALIDARG;
    }

    return __sx126XCMD(data, CMD_CALIBRATE_IMAGE, buff, 2, NULL, 0);
}

FUNCRESULT sx126XSetPAConfig(SX126XConfig *data, UINT8 paDutyCycle, UINT8 hpMax)
{
    BYTE buff[4];

    buff[0] = paDutyCycle;
    buff[1] = hpMax;
    buff[2] = 0x00;
    buff[3] = 0x01;

    return __sx126XCMD(data, CMD_SET_PA_CONFIG, buff, 4, NULL, 0);
}

FUNCRESULT sx126XSetRXTXFallbackMode(SX126XConfig *data, SX126XFallbackMode fallbackMode)
{
    BYTE buff[1];

    buff[0] = (BYTE)fallbackMode;

    return __sx126XCMD(data, CMD_SET_RX_TX_FALLBACK_MODE, buff, 1, NULL, 0);
}

FUNCRESULT sx126XWriteBuffer(SX126XConfig *data, UINT8 offset, BYTE *buffer, SIZE szBuffer)
{
    BYTE buff[16];

    buff[0] = (BYTE)offset;

    memcpy(buff + 1, buffer, szBuffer);

    return __sx126XCMD(data, CMD_WRITE_BUFFER, buff, szBuffer + 1, NULL, 0);
}

FUNCRESULT sx126XReadBuffer(SX126XConfig *data, UINT8 offset, BYTE *buffer, SIZE szBuffer)
{
    BYTE buff[1];

    buff[0] = (BYTE)offset;

    BYTE resBuff[16];

    __sx126XCMD(data, CMD_READ_BUFFER, buff, 1, resBuff, szBuffer + 1);

    memcpy(buffer, resBuff + 1, szBuffer);

    return SUC_OK;
}

FUNCRESULT sx126XSetDIOIRQParams(SX126XConfig *data, UINT16 irqMask, UINT16 dio1Mask, UINT16 dio2Mask, UINT16 dio3Mask)
{
    BYTE buff[8];

    buff[0] = (BYTE)(irqMask >> 8);
    buff[1] = (BYTE)(irqMask);
    buff[2] = (BYTE)(dio1Mask >> 8);
    buff[3] = (BYTE)(dio1Mask);
    buff[4] = (BYTE)(dio2Mask >> 8);
    buff[5] = (BYTE)(dio2Mask);
    buff[6] = (BYTE)(dio3Mask >> 8);
    buff[7] = (BYTE)(dio3Mask);

    return __sx126XCMD(data, CMD_SET_DIO_IRQ_PARAMS, buff, 8, NULL, 0);
}

FUNCRESULT sx126XGetIRQStatus(SX126XConfig *data, UINT16 *irqStatus)
{
    BYTE buff[3];

    FUNCRESULT res = __sx126XCMD(data, CMD_GET_IRQ_STATUS, NULL, 0, buff, 3);

    if (res == SUC_OK)
    {
        *irqStatus = (UINT16)((buff[1] << 8) | buff[2]);
    }

    return res;
}

FUNCRESULT sx126XClearIRQStatus(SX126XConfig *data, UINT16 clearIrqParam)
{
    BYTE buff[2];

    buff[0] = (BYTE)(clearIrqParam >> 8);
    buff[1] = (BYTE)(clearIrqParam);

    return __sx126XCMD(data, CMD_CLEAR_IRQ_STATUS, buff, 2, NULL, 0);
}

FUNCRESULT sx126XSetDIO2AsRFSwitchCtrl(SX126XConfig *data, BOOL enable)
{
    BYTE buff[1];

    buff[0] = (BYTE)enable;

    return __sx126XCMD(data, CMD_SET_DIO2_AS_RF_SWITCH_CTRL, buff, 1, NULL, 0);
}

FUNCRESULT sx126XSetDIO3AsTCXOCtrl(SX126XConfig *data, SX126XTCXOVoltage voltage, UINT32 timeout_ms)
{
    BYTE buff[4];

    UINT32 timeout = __sx126XConvertTimeoutToRTCStep(timeout_ms);

    buff[0] = (BYTE)voltage;
    buff[1] = (BYTE)(timeout >> 16);
    buff[2] = (BYTE)(timeout >> 8);
    buff[3] = (BYTE)(timeout);

    return __sx126XCMD(data, CMD_SET_DIO3_AS_TCXO_CTRL, buff, 3, NULL, 0);
}

FUNCRESULT sx126XSetRFFrequency(SX126XConfig *data, UINT32 frequency)
{
    BYTE buff[4];

    UINT32 freq = __sx126XConvertFrequencyToRegisterValue(frequency);

    buff[0] = (BYTE)(freq >> 24);
    buff[1] = (BYTE)(freq >> 16);
    buff[2] = (BYTE)(freq >> 8);
    buff[3] = (BYTE)(freq);

    return __sx126XCMD(data, CMD_SET_RF_FREQUENCY, buff, 4, NULL, 0);
}

FUNCRESULT sx126XSetPacketType(SX126XConfig *data, SX126XPacketType packetType)
{
    BYTE buff[1];

    buff[0] = (BYTE)packetType;

    return __sx126XCMD(data, CMD_SET_PACKET_TYPE, buff, 1, NULL, 0);
}

FUNCRESULT sx126XGetPacketType(SX126XConfig *data, SX126XPacketType *packetType)
{
    BYTE buff[2];

    FUNCRESULT res = __sx126XCMD(data, CMD_GET_PACKET_TYPE, NULL, 0, buff, 2);

    if (res == SUC_OK)
    {
        *packetType = (SX126XPacketType)buff[1];
    }

    return res;
}

FUNCRESULT sx126XSetTXParams(SX126XConfig *data, INT8 power, SX126XRampTime rampTime)
{
    BYTE buff[2];

    buff[0] = (BYTE)power;
    buff[1] = (BYTE)rampTime;

    return __sx126XCMD(data, CMD_SET_TX_PARAMS, buff, 2, NULL, 0);
}

FUNCRESULT sx126XSetGFSKModulationParams(SX126XConfig *data, UINT32 bitrate, SX126XGFSKPulseShape pulseShape, SX126XGFSKBandwidth bandwidth, UINT32 freq_dev_hz)
{
    BYTE buff[8];

    UINT32 br = (UINT32)(32 * (double)XTAL_FREQ / (double)bitrate);
    UINT32 freq_dev = __sx126XConvertFrequencyToRegisterValue(freq_dev_hz);

    buff[0] = (BYTE)(br >> 16);
    buff[1] = (BYTE)(br >> 8);
    buff[2] = (BYTE)(br);
    buff[3] = (BYTE)pulseShape;
    buff[4] = (BYTE)bandwidth;
    buff[5] = (BYTE)(freq_dev >> 16);
    buff[6] = (BYTE)(freq_dev >> 8);
    buff[7] = (BYTE)(freq_dev);

    return __sx126XCMD(data, CMD_SET_MODULATION_PARAMS, buff, 8, NULL, 0);
}

FUNCRESULT sx126XSetLoRaParams(SX126XConfig *data, SX126XLoRaSF sf, SX126XLoRaBW bw, SX126XLoRaCR cr, BOOL lowDataRateOptimize)
{
    BYTE buff[4];

    buff[0] = (BYTE)sf;
    buff[1] = (BYTE)bw;
    buff[2] = (BYTE)cr;
    buff[3] = (BYTE)lowDataRateOptimize;

    return __sx126XCMD(data, CMD_SET_PACKET_PARAMS, buff, 4, NULL, 0);
}

FUNCRESULT sx126XSetPacketGFSKParams(SX126XConfig *data, UINT16 preambleLength, SX126XGFSKPreambleDetectorLength detectorLength, UINT8 syncWordLength, SX126XGFSKAddressFiltering addressFiltering, SX126XGFSKPacketType packetType, UINT8 payloadLength, SX126XGFSKCRCType crcType, BOOL whitening)
{
    BYTE buff[9];

    buff[0] = (BYTE)(preambleLength >> 8);
    buff[1] = (BYTE)(preambleLength);
    buff[2] = (BYTE)detectorLength;
    buff[3] = (BYTE)syncWordLength;
    buff[4] = (BYTE)addressFiltering;
    buff[5] = (BYTE)packetType;
    buff[6] = (BYTE)payloadLength;
    buff[7] = (BYTE)crcType;
    buff[8] = (BYTE)whitening;

    return __sx126XCMD(data, CMD_SET_PACKET_PARAMS, buff, 9, NULL, 0);
}

FUNCRESULT sx126XSetPacketLoRaParams(SX126XConfig *data, UINT16 preambleLength, SX126XLoRaHeaderType headerType, UINT8 payloadLength, BOOL crc, BOOL invertIQ)
{
    BYTE buff[6];

    buff[0] = (BYTE)(preambleLength >> 8);
    buff[1] = (BYTE)(preambleLength);
    buff[2] = (BYTE)headerType;
    buff[3] = (BYTE)payloadLength;
    buff[4] = (BYTE)crc;
    buff[5] = (BYTE)invertIQ;

    return __sx126XCMD(data, CMD_SET_PACKET_PARAMS, buff, 6, NULL, 0);
}

FUNCRESULT sx126XSetCADParams(SX126XConfig *data, SX126XCADSymbol cadSymbolNum, UINT8 cadDetPeak, UINT8 cadDetMin, SX126XLoRaCADExitMode cadExitMode, UINT32 timeout_ms)
{
    BYTE buff[7];

    UINT32 timeout = __sx126XConvertTimeoutToRTCStep(timeout_ms);

    buff[0] = (BYTE)cadSymbolNum;
    buff[1] = (BYTE)cadDetPeak;
    buff[2] = (BYTE)cadDetMin;
    buff[3] = (BYTE)cadExitMode;
    buff[4] = (BYTE)(timeout >> 16);
    buff[5] = (BYTE)(timeout >> 8);
    buff[6] = (BYTE)(timeout);

    return __sx126XCMD(data, CMD_SET_CAD_PARAMS, buff, 7, NULL, 0);
}

FUNCRESULT sx126XSetBufferBaseAddress(SX126XConfig *data, UINT8 txBaseAddress, UINT8 rxBaseAddress)
{
    BYTE buff[2];

    buff[0] = (BYTE)txBaseAddress;
    buff[1] = (BYTE)rxBaseAddress;

    return __sx126XCMD(data, CMD_SET_BUFFER_BASE_ADDRESS, buff, 2, NULL, 0);
}

FUNCRESULT sx126XSetLoRaSymbNumTimeout(SX126XConfig *data, UINT8 symbNum)
{
    BYTE buff[1];

    buff[0] = (BYTE)symbNum;

    return __sx126XCMD(data, CMD_SET_LORA_SYMB_NUM_TIMEOUT, buff, 1, NULL, 0);
}

FUNCRESULT sx126XGetStatus(SX126XConfig *data, SX126XChipMode *pChipMode, SX126XCommandStatus *pCmdStatus)
{
    BYTE buff[1];

    FUNCRESULT res = __sx126XCMD(data, CMD_GET_STATUS, NULL, 0, buff, 1);

    if (res == SUC_OK)
    {
        BYTE d = buff[0];

        *pChipMode = (SX126XChipMode)((d & 0x70) >> 4);
        *pCmdStatus = (SX126XCommandStatus)((d & 0x0E) >> 1);
    }

    return res;
}

FUNCRESULT sx126XGetRXBufferStatus(SX126XConfig *data, UINT8 *pPayloadLength, UINT8 *pRXStartBufferPtr)
{
    BYTE buff[3];

    FUNCRESULT res = __sx126XCMD(data, CMD_GET_RX_BUFFER_STATUS, NULL, 0, buff, 3);

    if (res == SUC_OK)
    {
        *pPayloadLength = buff[1];
        *pRXStartBufferPtr = buff[2];
    }

    return res;
}

FUNCRESULT sx126XGetGFSKPacketStatus(SX126XConfig *data, SX126XGFSKPacketRXStatus *pRXStatus, INT8 *pRssiSync, INT8 *pRssiAvg)
{
    BYTE buff[4];

    FUNCRESULT res = __sx126XCMD(data, CMD_GET_PACKET_STATUS, NULL, 0, buff, 4);

    if (res == SUC_OK)
    {
        memcpy(pRXStatus, &buff[1], 1);

        *pRssiSync = (INT8)buff[2];
        *pRssiAvg = (INT8)buff[3];
    }

    return res;
}

FUNCRESULT sx126XGetLoRaPacketStatus(SX126XConfig *data, INT8 *pRssiPacket, INT8 *pSnrPkt, INT8 *pSignalRssiPkt)
{
    BYTE buff[4];

    FUNCRESULT res = __sx126XCMD(data, CMD_GET_PACKET_STATUS, NULL, 0, buff, 4);

    if (res == SUC_OK)
    {
        *pRssiPacket = (INT8)buff[1];
        *pSnrPkt = (INT8)buff[2];
        *pSignalRssiPkt = (INT8)buff[3];
    }

    return res;
}

FUNCRESULT sx126XGetRssiInst(SX126XConfig *data, INT8 *pRssiInst)
{
    BYTE buff[2];

    FUNCRESULT res = __sx126XCMD(data, CMD_GET_RSSI_INST, NULL, 0, buff, 2);

    if (res == SUC_OK)
    {
        *pRssiInst = (INT8)buff[1];
    }

    return res;
}

FUNCRESULT sx126XGetGFSKStats(SX126XConfig *data, UINT16 *pNbPktReceived, UINT16 *pNbPktCrcErr, UINT16 *pNbPktLenErr)
{
    BYTE buff[7];

    FUNCRESULT res = __sx126XCMD(data, CMD_GET_STATS, NULL, 0, buff, 7);

    if (res == SUC_OK)
    {
        *pNbPktReceived = (UINT16)((buff[1] << 8) | buff[2]);
        *pNbPktCrcErr = (UINT16)((buff[3] << 8) | buff[4]);
        *pNbPktLenErr = (UINT16)((buff[5] << 8) | buff[6]);
    }

    return res;
}

FUNCRESULT sx126XGetLoRaStats(SX126XConfig *data, UINT16 *pNbPktReceived, UINT16 *pNbPktCrcErr, UINT16 *pNbPktHeaderErr)
{
    BYTE buff[7];

    FUNCRESULT res = __sx126XCMD(data, CMD_GET_STATS, NULL, 0, buff, 7);

    if (res == SUC_OK)
    {
        *pNbPktReceived = (UINT16)((buff[1] << 8) | buff[2]);
        *pNbPktCrcErr = (UINT16)((buff[3] << 8) | buff[4]);
        *pNbPktHeaderErr = (UINT16)((buff[5] << 8) | buff[6]);
    }

    return res;
}

FUNCRESULT sx126XResetStats(SX126XConfig *data)
{
    BYTE buff[6];
    memset(buff, 0, 6);

    return __sx126XCMD(data, CMD_RESET_STATS, buff, 6, NULL, 0);
}

FUNCRESULT sx126XGetDeviceErrors(SX126XConfig *data, UINT16 *pErrors)
{
    BYTE buff[3];

    FUNCRESULT res = __sx126XCMD(data, CMD_GET_DEVICE_ERRORS, NULL, 0, buff, 3);

    if (res == SUC_OK)
    {
        *pErrors = (UINT16)((buff[1] << 8) | buff[2]);
    }

    return res;
}

FUNCRESULT sx126XClearDeviceErrors(SX126XConfig *data)
{
    BYTE buff[2];
    memset(buff, 0, 2);

    return __sx126XCMD(data, CMD_CLEAR_DEVICE_ERRORS, buff, 2, NULL, 0);
}

UINT32 __sx126XConvertTimeoutToRTCStep(UINT32 timeout_ms)
{
    return timeout_ms * RTC_FREQ / 1000;
}

UINT32 __sx126XConvertFrequencyToRegisterValue(UINT32 frequency)
{
    return (UINT32)((frequency / (double)XTAL_FREQ) * (double)(1 << 25));
}

VOID __sx126XWriteRegister(SX126XConfig *data, UINT16 address, BYTE *buffer, SIZE szBuffer)
{
    BYTE buff[16];

    buff[0] = (BYTE)(address >> 8);
    buff[1] = (BYTE)(address);

    memcpy(buff + 2, buffer, szBuffer);

    __sx126XCMD(data, CMD_WRITE_REGISTER, buff, szBuffer + 2, NULL, 0);
}

VOID __sx126XReadRegister(SX126XConfig *data, UINT16 address, BYTE *buffer, SIZE szBuffer)
{
    __sx126XCMD(data, CMD_READ_REGISTER, (BYTE *)&address, 2, buffer, szBuffer);
}

FUNCRESULT __sx126XCMD(SX126XConfig *data, BYTE command, BYTE *params, SIZE szParams, BYTE *resultBuffer, SIZE szBuffer)
{
    BYTE buff[16];

    buff[0] = command;

    if (params && szParams > 0)
    {
        memcpy(buff + 1, params, szParams);
    }

    gpioSetPinState(data->pinout.cs, GPIO_LOW);

    spiWriteBlocking(data->pinout.spi, buff, szParams + 1);

    if (resultBuffer && szBuffer > 0)
    {
        spiReadBlocking(data->pinout.spi, 0, resultBuffer, szBuffer);
    }

    gpioSetPinState(data->pinout.cs, GPIO_HIGH);

    return SUC_OK;
}