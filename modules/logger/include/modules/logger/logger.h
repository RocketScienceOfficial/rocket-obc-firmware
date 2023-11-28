#ifndef _LOGGER_H
#define _LOGGER_H

#ifndef NDEBUG

#include <stdio.h>

#define OBC_INFO(msg, ...) printf("[INFO] " msg "\n", ##__VA_ARGS__)
#define OBC_WARN(msg, ...) printf("[WARN] " msg "\n", ##__VA_ARGS__)
#define OBC_ERR(msg, ...) printf("[ERR] " msg "\n", ##__VA_ARGS__)

#else

#define OBC_INFO(msg, ...)
#define OBC_WARN(msg, ...)
#define OBC_ERR(msg, ...)

#endif

#endif