#include "ahrs.h"

static ahrs_data_t s_CurrentData;

void ahrs_init(void)
{
}

void ahrs_update(void)
{
}

const ahrs_data_t *ahrs_get_data(void)
{
    return &s_CurrentData;
}