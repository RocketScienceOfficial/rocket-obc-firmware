#ifndef _MULTICORE_H
#define _MULTICORE_H

#include <stddef.h>

#define MULTICORE_MAX_FIFOS 2 /** Max fifos count */

typedef void (*hal_core_func_t)(void); /** Type for core function */

/**
 * @brief Starts Core 1
 *
 * @param function Function to start on new core
 */
void hal_core_start_next(hal_core_func_t function);

/**
 * @brief Initialize one-way FIFO buffer for one core. Use each fifo for only one core for writing
 *
 * @param index Index of FIFO buffer
 * @param fifoElementSize Size of FIFO element
 * @param fifoElementsCount Number of FIFO elements
 */
void hal_core_init_fifo(size_t index, size_t fifoElementSize, size_t fifoElementsCount);

/**
 * @brief Sends data to other core
 *
 * @param index Index of FIFO buffer
 * @param data Data to send
 */
void hal_core_send_data(size_t index, void *data);

/**
 * @brief Receives data from other core
 *
 * @param index Index of FIFO buffer
 * @param data Data to receive
 */
void hal_core_receive_data(size_t index, void *data);

/**
 * @brief Sets current core as victim for pausing. Can be used for both
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

#endif