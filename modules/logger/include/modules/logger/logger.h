#ifndef _LOGGER_H
#define _LOGGER_H

#ifndef NDEBUG

#include "modules/drivers/hal/serial_driver.h"

#define OBC_INFO(msg, ...) hal_serial_printf("[INFO] " msg "\n", ##__VA_ARGS__)
#define OBC_WARN(msg, ...) hal_serial_printf("[WARN] " msg "\n", ##__VA_ARGS__)
#define OBC_ERR(msg, ...) hal_serial_printf("[ERR] " msg "\n", ##__VA_ARGS__)

#else

#define OBC_INFO(msg, ...)
#define OBC_WARN(msg, ...)
#define OBC_ERR(msg, ...)

#endif

#endif