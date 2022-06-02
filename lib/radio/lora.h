#pragma once 

#include "pico/stdlib.h"
#include "pico/binary_info.h"
#include "hardware/gpio.h"
#include "hardware/spi.h"

#define LORA_MAX_INSTANCES 2

#define PA_OUTPUT_RFO_PIN 0
#define PA_OUTPUT_PA_BOOST_PIN 1

typedef struct lora_pinout
{
    int spi;
    int sck;
    int miso;
    int mosi;
    int cs;
    int ss;
    int reset;
    int dio0;
} lora_pinout_t;

typedef struct lora_data
{
    lora_pinout_t pinout;
    long _frequency;
    int _packetIndex;
    int _implicitHeaderMode;
    void (*_onReceive)(int);
    void (*_onTxDone)();
} lora_data_t;

void loraInit(lora_data_t *data, lora_pinout_t *pinout);
int loraBegin(lora_data_t *data, long frequency);
void loraEnd(lora_data_t *data);
int loraBeginPacket(lora_data_t *data, int implicitHeader);
int loraEndPacket(lora_data_t *data, bool async);

int loraParsePacket(lora_data_t *data, int size);
int loraPacketRssi(lora_data_t *data);
float loraPacketSnr(lora_data_t *data);
long loraPacketFrequencyError(lora_data_t *data);

int loraRssi(lora_data_t *data);

size_t loraWrite(lora_data_t *data, uint8_t byte);
size_t loraWrite_s(lora_data_t *data, const uint8_t *buffer, size_t size);
size_t loraWrite_str(lora_data_t *data, const char* str);
size_t loraWrite_str_s(lora_data_t *data, const char *buffer, size_t size);

int loraAvailable(lora_data_t *data);
int loraRead(lora_data_t *data);
int loraPeek(lora_data_t *data);
void loraFlush(lora_data_t *data);

void loraOnReceive(lora_data_t *data, void (*callback)(int));
void loraOnTxDone(lora_data_t *data, void (*callback)());

void loraReceive(lora_data_t *data, int size);

void loraIdle(lora_data_t *data);
void loraSleep(lora_data_t *data);

void loraSetTxPower(lora_data_t *data, int level, int outputPin);
void loraSetFrequency(lora_data_t *data, long frequency);
void loraSetSpreadingFactor(lora_data_t *data, int sf);
void loraSetSignalBandwidth(lora_data_t *data, long sbw);
void loraSetCodingRate4(lora_data_t *data, int denominator);
void loraSetPreambleLength(lora_data_t *data, long length);
void loraSetSyncWord(lora_data_t *data, int sw);
void loraEnableCrc(lora_data_t *data);
void loraDisableCrc(lora_data_t *data);
void loraEnableInvertIQ(lora_data_t *data);
void loraDisableInvertIQ(lora_data_t *data);

void loraSetOCP(lora_data_t *data, uint8_t mA);

void loraSetGain(lora_data_t *data, uint8_t gain);

uint8_t loraRandom(lora_data_t *data);

void loraSetSPIFrequency(lora_data_t *data, uint32_t frequency);

void loraDumpRegisters(lora_data_t *data);

void __loraExplicitHeaderMode(lora_data_t *data);
void __loraImplicitHeaderMode(lora_data_t *data);

void __loraHandleDio0Rise(lora_data_t *data);
bool __loraIsTransmitting(lora_data_t *data);

int __loraGetSpreadingFactor(lora_data_t *data);
long __loraGetSignalBandwidth(lora_data_t *data);

void __loraSetLdoFlag(lora_data_t *data);

uint8_t __loraReadRegister(lora_data_t *data, uint8_t address);
void __loraWriteRegister(lora_data_t *data, uint8_t address, uint8_t value);
uint8_t __loraSingleTransfer(lora_data_t *data, uint8_t address, uint8_t value);

void __loraOnDio0Rise(uint, uint32_t);