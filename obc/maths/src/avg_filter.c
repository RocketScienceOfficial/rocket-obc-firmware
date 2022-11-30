#include "maths/avg_filter.h"

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

VOID filterAvgWeight(AvgFilterData *data, FLOAT *v)
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

    *v = *v * 0.5f + data->v1 * 0.3f + data->v2 * 0.2f;

    data->v2 = data->v1;
    data->v1 = *v;
}