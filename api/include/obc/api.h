#pragma once

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

typedef int8_t INT8;
typedef uint8_t UINT8;
typedef int16_t INT16;
typedef uint16_t UINT16;
typedef int32_t INT32;
typedef uint32_t UINT32;
typedef int64_t INT64;
typedef uint64_t UINT64;
typedef size_t SIZE;
typedef UINT8 SIZE8;
typedef UINT8 BYTE;

typedef float FLOAT;
typedef long double DOUBLE;

typedef void VOID;

typedef char CHAR;
typedef CHAR *STRING;

typedef bool BOOL;

#define FALSE 0
#define TRUE 1

typedef UINT32 FUNCRESULT; /** The result code of the function */

#define SUC_OK 0x000            /** Success code */
#define ERR_INVALIDARG 0x001    /** One or more arguments are invalid */
#define ERR_ACCESSDENIED 0x002  /** Access to operation denied */
#define ERR_POINTER 0x003       /** Pointer is not valid */
#define ERR_UNEXPECTED 0x004    /** Unexpected failure */
#define ERR_FAIL 0x005          /** Unspecified failure */
#define ERR_UNINITIALIZED 0x006 /** Unitialized data failure */

#define FUNCSUCCESS(x) ((x) == SUC_OK) /** Check if a function result is successful */
#define FUNCFAILED(x) ((x) != SUC_OK)  /** Check if a function has failed */

/**
 * @brief Check if bool is true and return ERR_FAIL if not
 */
#define FUNC_CHECK_BOOL(x)   \
    {                        \
        if ((x) == FALSE)    \
        {                    \
            return ERR_FAIL; \
        }                    \
    }
