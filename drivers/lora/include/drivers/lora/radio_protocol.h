#pragma once

#include <obc/api.h>

#define RADIO_PACKET_PARITY_SIZE 32
#define RADIO_PACKET_SIGNATURE_LENGTH 16
#define RADIO_PACKET_PAYLOAD_SIZE 128

/**
 * @brief Header of radio packet (INTERNAL USE)
 */
typedef struct RadioHeader
{
    BYTE signature[RADIO_PACKET_SIGNATURE_LENGTH]; /** Radio packet signature */
} RadioHeader;

/**
 * @brief Body of radio packet
 */
typedef struct RadioBody
{
    BYTE command;                            /** Packet command */
    BYTE _padding[3];                        /** Padding to align struct. Don't use it. */
    BYTE payload[RADIO_PACKET_PAYLOAD_SIZE]; /** Packet payload */
    SIZE payloadSize;                        /** Payload size */
} RadioBody;

/**
 * @brief Radio packet structure (INTERNAL USE)
 */
typedef struct RadioPacket
{
    RadioHeader header; /** Header */
    RadioBody body;     /** Body */
} RadioPacket;

/**
 * @brief Serialize and encrypt radio packet
 *
 * @param body Body of radio packet
 * @param pBuffer Pointer to buffer
 *
 * @return True if success
 */
BOOL serializeRadioPacket(RadioBody *body, BYTE *pBuffer);

/**
 * @brief Deserialize and decrypt radio packet
 *
 * @param buffer Buffer
 * @param pBody Body of radio packet
 *
 * @return True if success
 */
BOOL deserializeRadioPacket(BYTE *buffer, RadioBody *pBody);