#ifndef _DRIVERS_ERRORS_H
#define _DRIVERS_ERRORS_H

#include <stdint.h>

typedef uint8_t driver_error_t; /** Type for errors  */

#define OBC_NO_ERROR 0x00
#define OBC_UNKNOWN_ERROR 0x01
#define OBC_INVALID_ARG_ERROR 0x02

/**
 * @brief Add new error to queue
 *
 * @param err Error to add
 */
void drivers_errors_push(driver_error_t err);

/**
 * @brief Get and remove oldest error from queue
 *
 * @return Error
 */
driver_error_t drivers_errors_pop(void);

#endif