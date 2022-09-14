#pragma once

#include "pico/stdlib.h"
#include "pico/binary_info.h"
#include "hardware/gpio.h"
#include "hardware/spi.h"
#include <stdint.h>
#include <stdbool.h>

#define LORA_DEFAULT_TX_POWER 17
#define LORA_MAX_INSTANCES 2

#define PA_OUTPUT_RFO_PIN 0
#define PA_OUTPUT_PA_BOOST_PIN 1

/**
 * @brief Pinout data to use in the radio.
 */
typedef struct SX1278Pinout
{
    int spi;
    int sck;
    int miso;
    int mosi;
    int cs;
    int ss;
    int reset;
    int dio0;
} SX1278Pinout;

/**
 * @brief LoRa radio configuration. All changes are done internally, so keep it just for reference!
 */
typedef struct SX1278Data
{
    SX1278Pinout _pinout;
    int _txPower;
    long _frequency;
    int _packetIndex;
    int _implicitHeaderMode;
    void (*_onReceive)(int);
    void (*_onTxDone)();
} SX1278Data;

void loraInit(SX1278Data *data, SX1278Pinout *pinout);
int loraBegin(SX1278Data *data, long frequency);
void loraEnd(SX1278Data *data);
int loraBeginPacket(SX1278Data *data, int implicitHeader);
int loraEndPacket(SX1278Data *data, bool async);
int loraParsePacket(SX1278Data *data, int size);
int loraPacketRssi(SX1278Data *data);
float loraPacketSnr(SX1278Data *data);
long loraPacketFrequencyError(SX1278Data *data);
int loraRssi(SX1278Data *data);
size_t loraWrite(SX1278Data *data, uint8_t byte);
size_t loraWrite_s(SX1278Data *data, const uint8_t *buffer, size_t size);
size_t loraWrite_str(SX1278Data *data, const char *str);
size_t loraWrite_str_s(SX1278Data *data, const char *buffer, size_t size);
int loraAvailable(SX1278Data *data);
int loraRead(SX1278Data *data);
int loraPeek(SX1278Data *data);
void loraFlush(SX1278Data *data);
void loraOnReceive(SX1278Data *data, void (*callback)(int));
void loraOnTxDone(SX1278Data *data, void (*callback)());
void loraReceive(SX1278Data *data, int size);
void loraIdle(SX1278Data *data);
void loraSleep(SX1278Data *data);
void loraSetTxPower(SX1278Data *data, int level);
void loraSetFrequency(SX1278Data *data, long frequency);
void loraSetSpreadingFactor(SX1278Data *data, int sf);
void loraSetSignalBandwidth(SX1278Data *data, long sbw);
void loraSetCodingRate4(SX1278Data *data, int denominator);
void loraSetPreambleLength(SX1278Data *data, long length);
void loraSetSyncWord(SX1278Data *data, int sw);
void loraEnableCrc(SX1278Data *data);
void loraDisableCrc(SX1278Data *data);
void loraEnableInvertIQ(SX1278Data *data);
void loraDisableInvertIQ(SX1278Data *data);
void loraSetOCP(SX1278Data *data, uint8_t mA);
void loraSetGain(SX1278Data *data, uint8_t gain);
uint8_t loraRandom(SX1278Data *data);
void loraSetSPIFrequency(SX1278Data *data, uint32_t frequency);
void loraDumpRegisters(SX1278Data *data, uint8_t buffer[128]);
void __loraExplicitHeaderMode(SX1278Data *data);
void __loraImplicitHeaderMode(SX1278Data *data);
void __loraHandleDio0Rise(SX1278Data *data);
bool __loraIsTransmitting(SX1278Data *data);
int __loraGetSpreadingFactor(SX1278Data *data);
long __loraGetSignalBandwidth(SX1278Data *data);
void __loraSetLdoFlag(SX1278Data *data);
uint8_t __loraReadRegister(SX1278Data *data, uint8_t address);
void __loraWriteRegister(SX1278Data *data, uint8_t address, uint8_t value);
uint8_t __loraSingleTransfer(SX1278Data *data, uint8_t address, uint8_t value);
void __loraOnDio0Rise(uint, uint32_t);