#ifndef _IGN_H
#define _IGN_H

#include <stdint.h>
#include <stdbool.h>

/**
 * @brief Possible flags of igniters
 */
typedef enum ign_flags
{
    IGN_FLAG_IGN_1_CONT = 1 << 0,
    IGN_FLAG_IGN_2_CONT = 1 << 1,
    IGN_FLAG_IGN_3_CONT = 1 << 2,
    IGN_FLAG_IGN_4_CONT = 1 << 3,
    IGN_FLAG_IGN_1_STATE = 1 << 4,
    IGN_FLAG_IGN_2_STATE = 1 << 5,
    IGN_FLAG_IGN_3_STATE = 1 << 6,
    IGN_FLAG_IGN_4_STATE = 1 << 7,
} ign_flags_t;

/**
 * @brief Initialize igniters
 */
void ign_init(void);

/**
 * @brief Update igniters
 */
void ign_update(void);

/**
 * @brief Check if igniters are armed
 *
 * @return True if igniters are armed
 */
bool ign_is_armed(void);

/**
 * @brief Gets current flags (status) of igniters
 *
 * @return Flags
 */
uint8_t ign_get_flags(void);

#endif