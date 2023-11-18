#ifndef _MULTITHREADING_H
#define _MULTITHREADING_H

#include <stddef.h>
#include <stdbool.h>

typedef void (*core_func_t)(void); /** Type for thread function */

/**
 * @brief Starts Core 1
 *
 * @param function Function to start on new core
 * @param fifoElementSize Size of FIFO element
 * @param fifoElementsCount Number of FIFO elements
 * @return true if success
 */
bool core_start_next(core_func_t function, size_t fifoElementSize, size_t fifoElementsCount);

/**
 * @brief Sets current core as victim for pausing.
 */
void core_set_as_victim(void);

/**
 * @brief Starts lockout for victim core
 */
void core_start_lock(void);

/**
 * @brief Ends lockout for victim core
 */
void core_end_lock(void);

/**
 * @brief Sends data to other core
 *
 * @param data Data to send
 */
void core_send_data(void *data);

/**
 * @brief Receives data from other core
 *
 * @param data Data to receive
 * @return true if success
 */
bool core_receive_data(void *data);

#endif