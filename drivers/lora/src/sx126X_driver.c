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

FUNCRESULT sx126XInit(SX126XConfig *data, SX126XPinout *pinout, UINT64 frequency)
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

    __sx126XCMD(data, CMD_SET_SLEEP, buff, 1, NULL, 0);

    return SUC_OK;
}

FUNCRESULT sx126XSetStandby(SX126XConfig *data, BOOL rc)
{
    BYTE buff[1];

    buff[0] = (BYTE)rc;

    __sx126XCMD(data, CMD_SET_STANDBY, buff, 1, NULL, 0);

    return SUC_OK;
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

VOID __sx126XCMD(SX126XConfig *data, BYTE command, BYTE *params, SIZE szParams, BYTE *resultBuffer, SIZE szBuffer)
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
}