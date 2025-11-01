#ifndef _IGN_H
#define _IGN_H

#include <stdint.h>
#include <stdbool.h>

/**
 * @brief Continuity flags for igniters
 */
typedef enum ign_cont_flags
{
    IGN_CONT_FLAG_IGN_PRESENT = 1 << 0,
    IGN_CONT_FLAG_FUSE_WORKING = 1 << 1,
} ign_cont_flags_t;

/**
 * @brief Initialize igniters
 */
void ign_init(void);

/**
 * @brief Update igniters
 */
void ign_update(void);

/**
 * @brief Gets current flags of specified igniter
 *
 * @param ignNumber Number (1-4) of igniter
 * @return Flags (ign_cont_flags_t)
 */
uint8_t ign_get_cont_flags(uint8_t ignNumber);

/**
 * @brief Checks if specified igniter is has been fired
 *
 * @param ignNumber Number (1-4) of igniter
 * @return Has igniter been fired
 */
bool ign_is_fired(uint8_t ignNumber);

/**
 * @brief Checks if specified igniter has been tested
 *
 * @param ignNumber Number (1-4) of igniter
 * @return Has igniter been tested
 */
bool ign_is_tested(uint8_t ignNumber);

#endif