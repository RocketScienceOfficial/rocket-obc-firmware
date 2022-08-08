#pragma once

#include "utils/errors_types.h"
#include "radio.h"
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

/**
 * @brief Header of radio packet. (INTERNAL USE)
 */
typedef struct _RadioHeader
{
    /**
     * @brief Parity Size.
     */
    size_t _paritySize;

    /**
     * @brief Parity.
     */
    uint8_t *_parity;
} _RadioHeader;

/**
 * @brief Body of radio packet.
 */
typedef struct RadioBody
{
    /**
     * @brief Packet command.
     */
    uint8_t command;

    /**
     * @brief Padding to align struct. Don't use it.
     */
    uint8_t _padding[3];

    /**
     * @brief Payload size.
     */
    size_t payloadSize;

    /**
     * @brief Packet payload.
     */
    uint8_t *payload;
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
 *
 * @return Result code.
 */
FUNCRESULT serializeRadioPacket(RadioBody *body, uint8_t **buffer_out_ptr, size_t *size_out);

/**
 * @brief Deserialize radio packet.
 *
 * @param buffer_in Buffer.
 * @param size_in Size of buffer.
 * @param body_out Body of radio packet.
 * @param validationResult_out Validation result.
 *
 * @return Result code.
 */
FUNCRESULT deserializeRadioPacket(uint8_t *buffer, size_t size, RadioBody *body_out, bool *validationResult);

/**
 * @brief Sends radio packet using radio.
 *
 * @param lora LoRa data.
 * @param body Packet body.
 * @return Result code.
 */
FUNCRESULT radioSendPacket(LoRaData *lora, RadioBody *body);

/**
 * @brief Receives radio packet using radio.
 *
 * @param lora LoRa data.
 * @param available Is new radio packet available.
 * @param body Packet bodyto get.
 * @param validationResult Validation result ot get.
 * @return Result code.
 */
FUNCRESULT radioReceivePacket(LoRaData *lora, bool *available, RadioBody *body, bool *validationResult);

/**
 * @brief Clears radio body. (i.a payload)
 *
 * @param body Radio body to clear
 * @return Result code.
 */
FUNCRESULT radioClearPacket(RadioBody *body);