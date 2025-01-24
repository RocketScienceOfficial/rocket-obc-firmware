#ifndef _SERIAL_H
#define _SERIAL_H

#include <datalink.h>
#include <hal/serial_driver.h>

#ifndef NDEBUG
#define SERIAL_DEBUG_LOG(fmt, ...) hal_serial_printf(fmt "\r\n", ##__VA_ARGS__)
#else
#define SERIAL_DEBUG_LOG(fmt, ...)
#endif

/**
 * @brief Initialize serial input system
 */
void serial_init(void);

/**
 * @brief Update serial input system, gather data
 */
void serial_update(void);

/**
 * @brief Send message to serial port
 *
 * @param message Pointer to messsage
 */
void serial_send_message(const datalink_frame_structure_serial_t *message);

/**
 * @brief Get current frame received from serial port
 *
 * @return Frame or NULL
 */
const datalink_frame_structure_serial_t *serial_get_current_message(void);

#endif