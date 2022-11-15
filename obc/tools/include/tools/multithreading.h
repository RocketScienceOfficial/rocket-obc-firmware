#pragma once

#include "typedefs.h"

/**
 * @brief Type for thread function.
 */
typedef void (*PicoCoreFunction)(void);

/**
 * @brief Starts Core 1.
 *
 * @param function Function to start on new core.
 * @return true if success.
 */
BOOL startCore1(PicoCoreFunction function);

/**
 * @brief Sends data to other core.
 *
 * @param data Data to send.
 * @return true if success.
 */
BOOL sendToOtherCore(UINT32 data);

/**
 * @brief Receives data from other core.
 *
 * @param data Data to receive.
 * @return true if success.
 */
BOOL receiveFromOtherCore(UINT32 *data);