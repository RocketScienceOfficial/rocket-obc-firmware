#pragma once

#include "tools/typedefs.h"

/**
 * @brief Header of radio packet. (INTERNAL USE)
 */
typedef struct _RadioHeader
{
    /**
     * @brief Parity Size.
     */
    SIZE _paritySize;

    /**
     * @brief Parity.
     */
    BYTE *_parity;
} _RadioHeader;

/**
 * @brief Body of radio packet.
 */
typedef struct RadioBody
{
    /**
     * @brief Packet command.
     */
    BYTE command;

    /**
     * @brief Padding to align struct. Don't use it.
     */
    BYTE _padding[3];

    /**
     * @brief Payload size.
     */
    SIZE payloadSize;

    /**
     * @brief Packet payload.
     */
    BYTE *payload;
} RadioBody;

/**
 * @brief Radio packet structure. (INTERNAL USE)
 */
typedef struct _RadioPacket
{
    /**
     * @brief Header.
     */
    _RadioHeader _header;

    /**
     * @brief Body.
     */
    RadioBody _body;
} _RadioPacket;

/**
 * @brief Serialize radio packet.
 *
 * @param body_in Body of radio packet.
 * @param buffer_out_ptr Pointer to buffer.
 * @param size_out Size of buffer.
 */
VOID serializeRadioPacket(RadioBody *body, BYTE **buffer_out_ptr, SIZE *size_out);

/**
 * @brief Deserialize radio packet.
 *
 * @param buffer_in Buffer.
 * @param size_in Size of buffer.
 * @param body_out Body of radio packet.
 * @param validationResult_out Validation result.
 */
VOID deserializeRadioPacket(BYTE *buffer, SIZE size, RadioBody *body_out, BOOL *validationResult);

/**
 * @brief Clears radio body. (i.a payload)
 *
 * @param body Radio body to clear
 */
VOID radioClearPacket(RadioBody *body);