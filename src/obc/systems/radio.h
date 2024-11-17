#ifndef _RADIO_H
#define _RADIO_H

#include <datalink.h>

/**
 * @brief Initialize radio
 */
void radio_init(void);

/**
 * @brief Update radio
 */
void radio_update(void);

/**
 * @brief Get current frame from radio (from UART)
 *
 * @return Frame or NULL
 */
const datalink_frame_structure_serial_t *radio_get_current_message(void);

#endif