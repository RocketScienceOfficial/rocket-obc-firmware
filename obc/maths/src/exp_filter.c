#include "maths/exp_filter.h"

VOID filterExp(ExpFilterData *data, FLOAT *v, FLOAT alpha, FLOAT beta)
{
    if (!data || !v)
    {
        return;
    }

    if (!data->v1Init)
    {
        data->v1 = *v;
        data->v1Init = TRUE;

        return;
    }

    if (!data->bInit)
    {
        data->b = *v - data->v1;
        data->bInit = TRUE;
    }

    *v = alpha * (*v) + (1 - alpha) * (data->v1 + data->b);
    data->b = beta * (*v - data->v1) + (1 - beta) * data->b;
    data->v1 = *v;
}