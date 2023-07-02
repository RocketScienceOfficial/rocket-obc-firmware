#pragma once

#include <obc/api.h>

#define RADIO_PACKET_SIGNATURE_LENGTH 16

/**
 * @brief Header of radio packet (INTERNAL USE)
 */
typedef struct RadioHeader
{
    SIZE paritySize;                               /** Parity Size */
    BYTE *parity;                                  /** Parity */
    BYTE signature[RADIO_PACKET_SIGNATURE_LENGTH]; /** Radio packet signature */
} RadioHeader;

/**
 * @brief Body of radio packet
 */
typedef struct RadioBody
{
    BYTE command;     /** Packet command */
    BYTE _padding[3]; /** Padding to align struct. Don't use it. */
    SIZE payloadSize; /** Payload size */
    BYTE *payload;    /** Packet payload */
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
 * @param body_in Body of radio packet
 * @param buffer_out_ptr Pointer to buffer
 * @param size_out Size of buffer
 *
 * @return True if success
 */
BOOL serializeRadioPacket(RadioBody *body, BYTE **buffer_out_ptr, SIZE *size_out);

/**
 * @brief Deserialize and decrypt radio packet. Remeber to call radioClearPacket() on packet body after processing.
 *
 * @param buffer_in Buffer
 * @param size_in Size of buffer
 * @param body_out Body of radio packet
 *
 * @return True if success
 */
BOOL deserializeRadioPacket(BYTE *buffer, SIZE size, RadioBody *body_out);

/**
 * @brief Clears radio body (i.e payload)
 *
 * @param body Radio body to clear
 */
VOID radioClearPacket(RadioBody *body);