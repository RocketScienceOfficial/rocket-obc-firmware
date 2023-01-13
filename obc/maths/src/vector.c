#include "maths/vector.h"
#include <stdlib.h>
#include <math.h>

vec2 vec2Add(vec2 a, vec2 b)
{
    return (vec2){a.x + b.x, a.y + b.y};
}

vec2 vec2Sub(vec2 a, vec2 b)
{
    return (vec2){a.x - b.x, a.y - b.y};
}

FLOAT vec2Magnitude(vec2 v)
{
    return sqrt(v.x * v.x + v.y * v.y);
}

vec2 vec2Normalize(vec2 v)
{
    FLOAT magnitude = vec2Magnitude(v);

    return vec2MulNum(1.0f / magnitude, v);
}

vec2 vec2MulNum(FLOAT n, vec2 b)
{
    return (vec2){n * b.x, n * b.y};
}

vec2 vec2Dot(vec2 a, vec2 b)
{
    return (vec2){a.x * b.x, a.y * b.y};
}

vecN vec2ToVecN(vec2 v)
{
    vecN result = vecNCreate(2);

    result.data[0] = v.x;
    result.data[1] = v.y;

    return result;
}

VOID vec2ToVecNRef(vecN *vRef, vec2 v)
{
    vRef->data[0] = v.x;
    vRef->data[1] = v.y;
}

vec3 vec3Add(vec3 a, vec3 b)
{
    return (vec3){a.x + b.x, a.y + b.y, a.z + b.z};
}

vec3 vec3Sub(vec3 a, vec3 b)
{
    return (vec3){a.x - b.x, a.y - b.y, a.z - b.z};
}

FLOAT vec3Magnitude(vec3 v)
{
    return sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
}

vec3 vec3Normalize(vec3 v)
{
    FLOAT magnitude = vec3Magnitude(v);

    return vec3MulNum(1.0f / magnitude, v);
}

vec3 vec3MulNum(FLOAT n, vec3 b)
{
    return (vec3){n * b.x, n * b.y, n * b.z};
}

vec3 vec3Dot(vec3 a, vec3 b)
{
    return (vec3){a.x * b.x, a.y * b.y, a.z * b.z};
}

vecN vec3ToVecN(vec3 v)
{
    vecN result = vecNCreate(3);

    result.data[0] = v.x;
    result.data[1] = v.y;
    result.data[2] = v.z;

    return result;
}

VOID vec3ToVecNRef(vecN *vRef, vec3 v)
{
    vRef->data[0] = v.x;
    vRef->data[1] = v.y;
    vRef->data[2] = v.z;
}

vec4 vec4Add(vec4 a, vec4 b)
{
    return (vec4){a.x + b.x, a.y + b.y, a.z + b.z, a.w + b.w};
}

vec4 vec4Sub(vec4 a, vec4 b)
{
    return (vec4){a.x - b.x, a.y - b.y, a.z - b.z, a.w - b.w};
}

FLOAT vec4Magnitude(vec4 v)
{
    return sqrt(v.x * v.x + v.y * v.y + v.z * v.z + v.w * v.w);
}

vec4 vec4Normalize(vec4 v)
{
    FLOAT magnitude = vec4Magnitude(v);

    return vec4MulNum(1.0f / magnitude, v);
}

vec4 vec4MulNum(FLOAT n, vec4 b)
{
    return (vec4){n * b.x, n * b.y, n * b.z, n * b.w};
}

vec4 vec4Dot(vec4 a, vec4 b)
{
    return (vec4){a.x * b.x, a.y * b.y, a.z * b.z, a.w * b.w};
}

vecN vec4ToVecN(vec4 v)
{
    vecN result = vecNCreate(4);

    result.data[0] = v.x;
    result.data[1] = v.y;
    result.data[2] = v.z;
    result.data[3] = v.w;

    return result;
}

VOID vec4ToVecNRef(vecN *vRef, vec4 v)
{
    vRef->data[0] = v.x;
    vRef->data[1] = v.y;
    vRef->data[2] = v.z;
    vRef->data[3] = v.w;
}

vecN vecNCreate(SIZE size)
{
    FLOAT *data = (FLOAT *)calloc(size, sizeof(FLOAT));

    return (vecN){data, size};
}

VOID vecNFree(vecN v)
{
    if (v.data && v.size > 0)
    {
        free(v.data);

        v.data = NULL;
        v.size = 0;
    }
}

vecN vecNAdd(vecN a, vecN b)
{
    vecN result = vecNCreate(a.size);

    for (SIZE i = 0; i < a.size; i++)
    {
        result.data[i] = a.data[i] + b.data[i];
    }

    return result;
}

vecN vecNSub(vecN a, vecN b)
{
    vecN result = vecNCreate(a.size);

    for (SIZE i = 0; i < a.size; i++)
    {
        result.data[i] = a.data[i] - b.data[i];
    }

    return result;
}

FLOAT vecNMagnitude(vecN v)
{
    FLOAT result = 0.0f;

    for (SIZE i = 0; i < v.size; i++)
    {
        result += v.data[i] * v.data[i];
    }

    return sqrt(result);
}

vecN vecNNormalize(vecN v)
{
    FLOAT magnitude = vecNMagnitude(v);

    return vecNMulNum(1.0f / magnitude, v);
}

vecN vecNMulNum(FLOAT n, vecN b)
{
    vecN result = vecNCreate(b.size);

    for (SIZE i = 0; i < b.size; i++)
    {
        result.data[i] = n * b.data[i];
    }

    return result;
}

vecN vecNDot(vecN a, vecN b)
{
    vecN result = vecNCreate(a.size);

    for (SIZE i = 0; i < a.size; i++)
    {
        result.data[i] = a.data[i] * b.data[i];
    }

    return result;
}