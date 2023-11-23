#ifndef _MULTITHREADING_H
#define _MULTITHREADING_H

#include <stddef.h>
#include <stdbool.h>

typedef void (*hal_core_func_t)(void); /** Type for thread function */

/**
 * @brief Starts Core 1
 *
 * @param function Function to start on new core
 * @param fifoElementSize Size of FIFO element
 * @param fifoElementsCount Number of FIFO elements
 * @return true if success
 */
bool hal_core_start_next(hal_core_func_t function, size_t fifoElementSize, size_t fifoElementsCount);

/**
 * @brief Sets current core as victim for pausing.
 */
void hal_core_set_as_victim(void);

/**
 * @brief Starts lockout for victim core
 */
void hal_core_start_lock(void);

/**
 * @brief Ends lockout for victim core
 */
void hal_core_end_lock(void);

/**
 * @brief Sends data to other core
 *
 * @param data Data to send
 */
void hal_core_send_data(void *data);

/**
 * @brief Receives data from other core
 *
 * @param data Data to receive
 * @return true if success
 */
bool hal_core_receive_data(void *data);

#endif