#ifndef _SYSLOG_H
#define _SYSLOG_H

#include "hal/serial_driver.h"

#define SYS_LOG(log, ...) hal_serial_printf("[" SYSTEM_NAME "]: " log "\n", ##__VA_ARGS__) /** Log to serial with some info */

#endif