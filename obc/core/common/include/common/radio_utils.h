#pragma once

#include <obc/api.h>
#include "drivers/lora/sx1278_driver.h"
#include "kernel/services/radio_protocol.h"

#define RADIO_DBM 20
#define RADIO_SPREADING_FACTOR 10
#define RADIO_SIGNAL_BANDWIDTH 125E3
#define RADIO_ERROR_SPREADING_FACTOR 10
#define RADIO_ERROR_SIGNAL_BANDWIDTH 125E3
#define RADIO_FREQUENCY_HZ 433E6
#define RADIO_MIN_RSSI -80

#define MEASUREMENTS_RADIO_COMMAND_ID 'M'
#define COMMANDS_RADIO_COMMAND_ID 'C'

/**
 * @brief Radio packet wrapper structure
 */
typedef struct RadioUtilPacketData
{
    RadioBody body;       /** Packet body */
    INT32 signalStrength; /** RSSI.*/
} RadioUtilPacketData;

/**
 * @brief Initialize radio module
 *
 * @param pinout Radio pinout
 */
VOID initializeRadio(SX1278Pinout *pinout);

/**
 * @brief Check if radio packet is available
 *
 * @param packet Radio packet is available. Remeber to call radioClearPacket() on packet body after processing.
 * @return TRUE if radio packet is available, FALSE otherwise
 */
BOOL checkRadioPacket(RadioUtilPacketData *packet);

/**
 * @brief Send radio packet
 *
 * @param body Radio packet body
 */
VOID sendRadioPacket(RadioBody *body);

/**
 * @brief Send command to be executed on other device
 *
 * @param msg Command message
 */
VOID sendRadioRemoteCommand(STRING msg);

/**
 * @brief Call this function when you receive radio packet with commnd
 *
 * @param msg Command message
 * @param size Size of command message
 */
VOID radioRemoteCommandCallback(BYTE *msg, SIZE size);