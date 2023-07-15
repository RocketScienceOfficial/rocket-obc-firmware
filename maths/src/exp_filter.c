#include "maths/exp_filter.h"

VOID filterExpInit(ExpFilterData *data, FLOAT a)
{
    data->a = a;
    data->vInit = FALSE;
}

VOID filterExpUpdate(ExpFilterData *data, FLOAT *v)
{
    if (!data->vInit)
    {
        data->v = *v;
        data->vInit = TRUE;

        return;
    }

    *v = data->a * *v + (1.0f - data->a) * data->v;
}