#pragma once

#include <obc/api.h>

typedef VOID (*PicoCoreFunction)(VOID); /** Type for thread function */

/**
 * @brief Starts Core 1
 *
 * @param function Function to start on new core
 * @param fifoElementSize Size of FIFO element
 * @param fifoElementsCount Number of FIFO elements
 * @return true if success
 */
BOOL startOtherCore(PicoCoreFunction function, SIZE fifoElementSize, SIZE fifoElementsCount);

/**
 * @brief Sets current core as victim for pausing.
 */
VOID setAsVictimCore(VOID);

/**
 * @brief Starts lockout for victim core
 */
VOID coreStartLockout(VOID);

/**
 * @brief Ends lockout for victim core
 */
VOID coreEndLockout(VOID);

/**
 * @brief Sends data to other core
 *
 * @param data Data to send
 * @return true if success
 */
BOOL sendToOtherCore(VOID *data);

/**
 * @brief Receives data from other core
 *
 * @param data Data to receive
 * @return true if success
 */
BOOL receiveFromOtherCore(VOID *data);