#include "pico/stdlib.h"
#include "pico/binary_info.h"
#include "hardware/gpio.h"
#include "hardware/spi.h"
#include "pinout_config.h"

#define PIN_MISO SX1278_MISO_GPIO
#define PIN_CS SX1278_CS_GPIO
#define PIN_SCK SX1278_SCK_GPIO
#define PIN_MOSI SX1278_MOSI_GPIO

#define SPI_PORT SX1278_SPI
#define READ_BIT 0x80

#define LORA_DEFAULT_SPI spi0
#define LORA_DEFAULT_SPI_FREQUENCY 8E6
#define LORA_DEFAULT_SS_PIN 8
#define LORA_DEFAULT_RESET_PIN 9
#define LORA_DEFAULT_DIO0_PIN 7

#define PA_OUTPUT_RFO_PIN 0
#define PA_OUTPUT_PA_BOOST_PIN 1

typedef struct lora_data
{
  spi_inst_t *_spi;
  int _ss;
  int _reset;
  int _dio0;
  long _frequency;
  int _packetIndex;
  int _implicitHeaderMode;
  void (*_onReceive)(int);
  void (*_onTxDone)();
} lora_data_t;

int loraBegin(lora_data_t *data, long frequency);
void loraEnd(lora_data_t *data);

int loraBeginPacket(lora_data_t *data, int implicitHeader = false);
int loraEndPacket(lora_data_t *data, bool async = false);

int loraParsePacket(lora_data_t *data, int size = 0);
int loraPacketRssi(lora_data_t *data);
float loraPacketSnr(lora_data_t *data);
long loraPacketFrequencyError(lora_data_t *data);

int loraRssi(lora_data_t *data);

size_t loraWrite(lora_data_t *data, uint8_t byte);
size_t loraWrite(lora_data_t *data, const uint8_t *buffer, size_t size);

int loraAailable(lora_data_t *data);
int loraRead(lora_data_t *data);
int loraPeek(lora_data_t *data);
void loraFlush(lora_data_t *data);

void loraOnReceive(lora_data_t *data, void (*callback)(int));
void loraOnTxDone(lora_data_t *data, void (*callback)());

void loraReceive(lora_data_t *data, int size = 0);

void loraIdle(lora_data_t *data);
void loraSleep(lora_data_t *data);

void loraSetTxPower(int level, int outputPin = PA_OUTPUT_PA_BOOST_PIN);
void loraSetFrequency(long frequency);
void loraSetSpreadingFactor(int sf);
void loraSetSignalBandwidth(long sbw);
void loraSetCodingRate4(int denominator);
void loraSetPreambleLength(long length);
void loraSetSyncWord(int sw);
void loraEnableCrc();
void loraDisableCrc();
void loraEnableInvertIQ();
void loraDisableInvertIQ();

void loraSetOCP(uint8_t mA);

void loraSetGain(uint8_t gain);

void loraCrc() { loraEnableCrc(); }
void loraNoCrc() { loraDisableCrc(); }

uint8_t loraRandom();

void loraSetPins(int ss = LORA_DEFAULT_SS_PIN, int reset = LORA_DEFAULT_RESET_PIN, int dio0 = LORA_DEFAULT_DIO0_PIN);
void loraSetSPI(spi_inst_t &spi);
void loraSetSPIFrequency(uint32_t frequency);

void loraDumpRegisters();

void loraExplicitHeaderMode();
void loraImplicitHeaderMode();

void loraHandleDio0Rise();
bool loraIsTransmitting();

int loraGetSpreadingFactor();
long loraGetSignalBandwidth();

void loraSetLdoFlag();

uint8_t loraReadRegister(uint8_t address);
void loraWriteRegister(uint8_t address, uint8_t value);
uint8_t loraSingleTransfer(uint8_t address, uint8_t value);

void loraOnDio0Rise(uint, uint32_t);