#pragma once

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

typedef int8_t INT8;
typedef uint8_t UINT8;
typedef UINT8 BYTE;
typedef char CHAR;
typedef int16_t INT16;
typedef int32_t INT32;
typedef int64_t INT64;
typedef uint16_t UINT16;
typedef uint32_t UINT32;
typedef uint64_t UINT64;
typedef size_t SIZE;
typedef float FLOAT;
typedef bool BOOL;
typedef void VOID;
typedef CHAR *STRING;

#define FALSE 0
#define TRUE 1

/**
 * @brief The result code of the function.
 */
typedef UINT32 FUNCRESULT;

/**
 * @brief Success code.
 */
#define SUC_OK 0x000

/**
 * @brief One or more arguments are invalid.
 */
#define ERR_INVALIDARG 0x001

/**
 * @brief Access to operation denied.
 */
#define ERR_ACCESSDENIED 0x002

/**
 * @brief Pointer is not valid.
 */
#define ERR_POINTER 0x003

/**
 * @brief Unexpected failure.
 */
#define ERR_UNEXPECTED 0x004

/**
 * @brief Unspecified failure.
 */
#define ERR_FAIL 0x005

/**
 * @brief Unitialized data failure.
 */
#define ERR_UNINITIALIZED 0x006

/**
 * @brief Check if a function result is successful.
 */
#define FUNCSUCCESS(x) ((x) == SUC_OK)

/**
 * @brief Check if a function has failed.
 */
#define FUNCFAILED(x) ((x) != SUC_OK)