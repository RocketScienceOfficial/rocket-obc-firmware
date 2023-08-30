// #include "radio_utils.h"
// #include "config.h"
// #include "driver_calling.h"
// #include "drivers/lora/sx126X_driver.h"
// #include <string.h>

// #define RADIO_FREQ 433E6
// #define RADIO_POWER 20

// static SX126XConfig s_LoraData;

// VOID initRadio(VOID)
// {
//     SX126XPinout loraPinout = (SX126XPinout){
//         .spi = SPI,
//         .sck = SCK,
//         .miso = MISO,
//         .mosi = MOSI,
//         .cs = LORA_CS_PIN,
//         .reset = LORA_RESET_PIN,
//         .busy = LORA_BUSY_PIN,
//     };

//     DRIVER_CALL(sx126XClearIRQStatus(&s_LoraData, SX126X_IRQ_RADIO_ALL_MASK));
//     DRIVER_CALL(sx126XSetStandby(&s_LoraData, SX126X_STANDBY_RC));
//     DRIVER_CALL(sx126XSetRegulatorMode(&s_LoraData, SX126X_REGULATOR_DC_DC));
//     DRIVER_CALL(sx126XSetPAConfig(&s_LoraData, 0x04, 0x07));
//     DRIVER_CALL(sx126XCalibrate(&s_LoraData, SX126X_CALIBRATE_ALL_MASK));
//     DRIVER_CALL(sx126XCalibrateImage(&s_LoraData, RADIO_FREQ));
//     DRIVER_CALL(sx126XSetPacketType(&s_LoraData, SX126X_PACKET_TYPE_LORA));
//     DRIVER_CALL(sx126XSetRFFrequency(&s_LoraData, RADIO_FREQ));
//     DRIVER_CALL(sx126XSetLoRaModulationParams(&s_LoraData, SX126X_LORA_SF_8, SX126X_LORA_BW_125, SX126X_LORA_CR_4_5, TRUE));
//     DRIVER_CALL(sx126XSetBufferBaseAddress(&s_LoraData, 0x00, 0x00));
//     DRIVER_CALL(sx126XSetPacketLoRaParams(&s_LoraData, 8, SX126X_LORA_HEADER_EXPLICIT, 255, TRUE, FALSE));
//     DRIVER_CALL(sx126XSetTXParams(&s_LoraData, RADIO_POWER, SX126X_RAMP_10U));
// }

// VOID sendRadioPacket(RadioBody *body)
// {
//     BYTE packetBuffer[sizeof(RadioPacket)];
//     BOOL serializationResult = serializeRadioPacket(body, packetBuffer);

//     if (serializationResult)
//     {
//         DRIVER_CALL(sx126XSetStandby(&s_LoraData, SX126X_STANDBY_RC));
//         DRIVER_CALL(sx126XSetBufferBaseAddress(&s_LoraData, 0x00, 0x00));
//         DRIVER_CALL(sx126XWriteBuffer(&s_LoraData, 0x00, packetBuffer, sizeof(RadioPacket)));
//         DRIVER_CALL(sx126XSetPacketLoRaParams(&s_LoraData, 8, SX126X_LORA_HEADER_EXPLICIT, sizeof(RadioPacket), TRUE, FALSE));
//         DRIVER_CALL(sx126XSetTX(&s_LoraData, 0));
//     }
// }

// VOID sendMeasurementData(MeasurementData *data)
// {
//     RadioBody body = {0};

//     body.command = MEASUREMENTS_RADIO_COMMAND_ID;
//     body.payloadSize = sizeof(*data);

//     memcpy(body.payload, data, body.payloadSize);

//     sendRadioPacket(&body);
// }

#include "radio_utils.h"
#include "config.h"
#include "driver_calling.h"
#include "drivers/lora/sx127X_driver.h"
#include <string.h>

#define RADIO_FREQ 433E6
#define RADIO_POWER 20

static SX127XData s_LoraData;

VOID initRadio(VOID)
{
    SX127XPinout loraPinout = (SX127XPinout){
        .spi = SPI,
        .sck = SCK,
        .miso = MISO,
        .mosi = MOSI,
        .cs = EXTERNAL_CS_PIN,
        .reset = LORA_RESET_PIN,
    };

    DRIVER_CALL(sx127XInit(&s_LoraData, &loraPinout, RADIO_FREQ));
    DRIVER_CALL(sx127XSetTxPower(&s_LoraData, RADIO_POWER));
    DRIVER_CALL(sx127XSetSpreadingFactor(&s_LoraData, 8));
    DRIVER_CALL(sx127XSetSignalBandwidth(&s_LoraData, 125000));
}

VOID sendRadioPacket(RadioBody *body)
{
    BYTE packetBuffer[sizeof(RadioPacket)];
    BOOL serializationResult = serializeRadioPacket(body, packetBuffer);

    if (serializationResult)
    {
        DRIVER_CALL(sx127XWriteBuffer(&s_LoraData, packetBuffer, sizeof(RadioPacket)));
    }
}

VOID sendMeasurementData(MeasurementData *data)
{
    RadioBody body = {0};

    body.command = MEASUREMENTS_RADIO_COMMAND_ID;
    body.payloadSize = sizeof(*data);

    memcpy(body.payload, data, body.payloadSize);

    sendRadioPacket(&body);
}