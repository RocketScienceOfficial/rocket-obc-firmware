#include "lib/drivers/utils/drivers_errors.h"

static driver_error_t s_Errors[32];
static uint8_t s_CurrentSize;

void drivers_errors_push(driver_error_t err)
{
    if (s_CurrentSize < sizeof(s_Errors) / sizeof(driver_error_t))
    {
        s_Errors[s_CurrentSize++] = err;
    }
}

driver_error_t drivers_errors_pop(void)
{
    if (s_CurrentSize == 0 || s_CurrentSize >= sizeof(s_Errors) / sizeof(driver_error_t))
    {
        return OBC_NO_ERROR;
    }

    driver_error_t err = s_Errors[0];

    for (uint8_t i = 1; i < s_CurrentSize; i++)
    {
        s_Errors[i - 1] = s_Errors[i];
    }

    s_CurrentSize--;

    return err;
}