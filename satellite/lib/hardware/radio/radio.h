#pragma once

#include <stdint.h>
#include <stdbool.h>
#include "pico/stdlib.h"
#include "pico/binary_info.h"
#include "hardware/gpio.h"
#include "hardware/spi.h"

#define LORA_DEFAULT_TX_POWER 17
#define LORA_MAX_INSTANCES 2

#define PA_OUTPUT_RFO_PIN 0
#define PA_OUTPUT_PA_BOOST_PIN 1

/**
 * @brief Pinout data to use in the radio.
 */
typedef struct LoRaPinout
{
    int spi;
    int sck;
    int miso;
    int mosi;
    int cs;
    int ss;
    int reset;
    int dio0;
} LoRaPinout;

/**
 * @brief LoRa radio configuration. All changes are done internally, so keep it just for reference!
 */
typedef struct LoRaData
{
    LoRaPinout _pinout;
    int _txPower;
    long _frequency;
    int _packetIndex;
    int _implicitHeaderMode;
    void (*_onReceive)(int);
    void (*_onTxDone)();
} LoRaData;

void loraInit(LoRaData *data, LoRaPinout *pinout);
int loraBegin(LoRaData *data, long frequency);
void loraEnd(LoRaData *data);
int loraBeginPacket(LoRaData *data, int implicitHeader);
int loraEndPacket(LoRaData *data, bool async);

int loraParsePacket(LoRaData *data, int size);
int loraPacketRssi(LoRaData *data);
float loraPacketSnr(LoRaData *data);
long loraPacketFrequencyError(LoRaData *data);

int loraRssi(LoRaData *data);

size_t loraWrite(LoRaData *data, uint8_t byte);
size_t loraWrite_s(LoRaData *data, const uint8_t *buffer, size_t size);
size_t loraWrite_str(LoRaData *data, const char *str);
size_t loraWrite_str_s(LoRaData *data, const char *buffer, size_t size);

int loraAvailable(LoRaData *data);
int loraRead(LoRaData *data);
int loraPeek(LoRaData *data);
void loraFlush(LoRaData *data);

void loraOnReceive(LoRaData *data, void (*callback)(int));
void loraOnTxDone(LoRaData *data, void (*callback)());

void loraReceive(LoRaData *data, int size);

void loraIdle(LoRaData *data);
void loraSleep(LoRaData *data);

void loraSetTxPower(LoRaData *data, int level);
void loraSetFrequency(LoRaData *data, long frequency);
void loraSetSpreadingFactor(LoRaData *data, int sf);
void loraSetSignalBandwidth(LoRaData *data, long sbw);
void loraSetCodingRate4(LoRaData *data, int denominator);
void loraSetPreambleLength(LoRaData *data, long length);
void loraSetSyncWord(LoRaData *data, int sw);
void loraEnableCrc(LoRaData *data);
void loraDisableCrc(LoRaData *data);
void loraEnableInvertIQ(LoRaData *data);
void loraDisableInvertIQ(LoRaData *data);

void loraSetOCP(LoRaData *data, uint8_t mA);

void loraSetGain(LoRaData *data, uint8_t gain);

uint8_t loraRandom(LoRaData *data);

void loraSetSPIFrequency(LoRaData *data, uint32_t frequency);

void loraDumpRegisters(LoRaData *data, uint8_t buffer[128]);

void __loraExplicitHeaderMode(LoRaData *data);
void __loraImplicitHeaderMode(LoRaData *data);

void __loraHandleDio0Rise(LoRaData *data);
bool __loraIsTransmitting(LoRaData *data);

int __loraGetSpreadingFactor(LoRaData *data);
long __loraGetSignalBandwidth(LoRaData *data);

void __loraSetLdoFlag(LoRaData *data);

uint8_t __loraReadRegister(LoRaData *data, uint8_t address);
void __loraWriteRegister(LoRaData *data, uint8_t address, uint8_t value);
uint8_t __loraSingleTransfer(LoRaData *data, uint8_t address, uint8_t value);

void __loraOnDio0Rise(uint, uint32_t);