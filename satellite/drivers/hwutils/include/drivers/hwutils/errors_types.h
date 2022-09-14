#pragma once

/**
 * @brief The result code of the function.
 */
typedef int FUNCRESULT;

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