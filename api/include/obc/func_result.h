#pragma once

#include "typedefs.h"

typedef UINT32 FUNCRESULT; /** The result code of the function */

#define SUC_OK 0x000                   /** Success code */
#define ERR_INVALIDARG 0x001           /** One or more arguments are invalid */
#define ERR_ACCESSDENIED 0x002         /** Access to operation denied */
#define ERR_POINTER 0x003              /** Pointer is not valid */
#define ERR_UNEXPECTED 0x004           /** Unexpected failure */
#define ERR_FAIL 0x005                 /** Unspecified failure */
#define ERR_UNINITIALIZED 0x006        /** Unitialized data failure */
#define FUNCSUCCESS(x) ((x) == SUC_OK) /** Check if a function result is successful */
#define FUNCFAILED(x) ((x) != SUC_OK)  /** Check if a function has failed */