#ifndef _UBX_H
#define _UBX_H

#include <stdint.h>
#include <stddef.h>

/**
 * @brief Create UBX packet. Read datasheet for reference.
 *
 * @param frameBuffer Buffer to write to
 * @param class Message class
 * @param id Message id
 * @param payload Payload
 * @param length Length
 * @return Size of the new packet
 */
size_t ubx_create_frame(uint8_t *frameBuffer, uint8_t class, uint8_t id, const uint8_t *payload, uint16_t length);

/**
 * @brief Calculate UBX checksum
 *
 * @param cka Pointer to first part of checksum
 * @param ckb Pointer to second part of checksum
 * @param buffer Buffer
 * @param len Length of buffer
 */
void ubx_checksum(uint8_t *cka, uint8_t *ckb, const uint8_t *buffer, uint8_t len);

#endif