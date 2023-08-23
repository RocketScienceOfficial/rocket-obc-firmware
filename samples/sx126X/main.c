#include "drivers/lora/sx126X_driver.h"
#include "drivers/tools/board_control.h"
#include <stdio.h>

#define SPI 1
#define SCK 10
#define MISO 11
#define MOSI 12
#define LORA_CS_PIN 17
#define LORA_RESET_PIN 18
#define LORA_BUSY_PIN 19
#define LORA_DIO1_PIN 20
#define RADIO_FREQ 433E6
#define RADIO_POWER 20

static SX126XConfig s_LoraData;

int main()
{
    boardInit(5000);

    spiInitAll(SPI, 1000000);

    SX126XPinout loraPinout = (SX126XPinout){
        .spi = SPI,
        .sck = SCK,
        .miso = MISO,
        .mosi = MOSI,
        .cs = LORA_CS_PIN,
        .reset = LORA_RESET_PIN,
        .busy = LORA_BUSY_PIN,
    };

    FUNCRESULT res;

    res = sx126XClearIRQStatus(&s_LoraData, SX126X_IRQ_RADIO_ALL_MASK);
    res = sx126XSetStandby(&s_LoraData, SX126X_STANDBY_RC);
    res = sx126XSetRegulatorMode(&s_LoraData, SX126X_REGULATOR_DC_DC);
    res = sx126XSetPAConfig(&s_LoraData, 0x04, 0x07);
    res = sx126XCalibrate(&s_LoraData, SX126X_CALIBRATE_ALL_MASK);
    res = sx126XCalibrateImage(&s_LoraData, RADIO_FREQ);
    res = sx126XSetPacketType(&s_LoraData, SX126X_PACKET_TYPE_LORA);
    res = sx126XSetRFFrequency(&s_LoraData, RADIO_FREQ);
    res = sx126XSetLoRaModulationParams(&s_LoraData, SX126X_LORA_SF_8, SX126X_LORA_BW_125, SX126X_LORA_CR_4_5, TRUE);
    res = sx126XSetBufferBaseAddress(&s_LoraData, 0x00, 0x00);
    res = sx126XSetPacketLoRaParams(&s_LoraData, 8, SX126X_LORA_HEADER_EXPLICIT, 255, TRUE, FALSE);
    res = sx126XSetTXParams(&s_LoraData, RADIO_POWER, SX126X_RAMP_10U);

    printf("Result: %d\n", res);

    while (TRUE)
    {
        tightLoop();
    }

    return 0;
}