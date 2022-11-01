#include "tools/filters.h"

VOID filterAvg(AvgFilterData *data, FLOAT *v)
{
    if (!data || !v)
    {
        return;
    }

    if (!data->v2Init)
    {
        data->v2 = *v;
        data->v2Init = TRUE;

        return;
    }

    if (!data->v1Init)
    {
        data->v1 = *v;
        data->v1Init = TRUE;

        return;
    }

    *v = (*v + data->v1 + data->v2) / 3;

    data->v2 = data->v1;
    data->v1 = *v;
}

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