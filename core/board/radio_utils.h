#pragma once

#include <obc/api.h>
#include "drivers/lora/radio_protocol.h"
#include "measurements.h"

#define MEASUREMENTS_RADIO_COMMAND_ID 'M'

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
 */
VOID initRadio(VOID);

/**
 * @brief Send radio packet
 *
 * @param body Radio packet body
 */
VOID sendRadioPacket(RadioBody *body);

/**
 * @brief Send measurement data
 *
 * @param data Measurement data
 */
VOID sendMeasurementData(MeasurementData *data);