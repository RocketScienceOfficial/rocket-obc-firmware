#include "maths/vector.h"
#include <stdlib.h>

vec2 vec2Add(vec2 a, vec2 b)
{
    return (vec2){a.x + b.x, a.y + b.y};
}

vec2 vec2Sub(vec2 a, vec2 b)
{
    return (vec2){a.x - b.x, a.y - b.y};
}

vec2 vec2MulNum(FLOAT n, vec2 b)
{
    return (vec2){n * b.x, n * b.y};
}

vec2 vec2Dot(vec2 a, vec2 b)
{
    return (vec2){a.x * b.x, a.y * b.y};
}

vec3 vec3Add(vec3 a, vec3 b)
{
    return (vec3){a.x + b.x, a.y + b.y, a.z + b.z};
}

vec3 vec3Sub(vec3 a, vec3 b)
{
    return (vec3){a.x - b.x, a.y - b.y, a.z - b.z};
}

vec3 vec3MulNum(FLOAT n, vec3 b)
{
    return (vec3){n * b.x, n * b.y, n * b.z};
}

vec3 vec3Dot(vec3 a, vec3 b)
{
    return (vec3){a.x * b.x, a.y * b.y, a.z * b.z};
}

vec4 vec4Add(vec4 a, vec4 b)
{
    return (vec4){a.x + b.x, a.y + b.y, a.z + b.z, a.w + b.w};
}

vec4 vec4Sub(vec4 a, vec4 b)
{
    return (vec4){a.x - b.x, a.y - b.y, a.z - b.z, a.w - b.w};
}

vec4 vec4MulNum(FLOAT n, vec4 b)
{
    return (vec4){n * b.x, n * b.y, n * b.z, n * b.w};
}

vec4 vec4Dot(vec4 a, vec4 b)
{
    return (vec4){a.x * b.x, a.y * b.y, a.z * b.z, a.w * b.w};
}

vecN vecNAdd(vecN a, vecN b)
{
    vecN result = {malloc(a.size * sizeof(FLOAT)), a.size};

    for (SIZE i = 0; i < a.size; i++)
    {
        result.data[i] = a.data[i] + b.data[i];
    }

    return result;
}

vecN vecNSub(vecN a, vecN b)
{
    vecN result = {malloc(a.size * sizeof(FLOAT)), a.size};

    for (SIZE i = 0; i < a.size; i++)
    {
        result.data[i] = a.data[i] - b.data[i];
    }

    return result;
}

vecN vecNMulNum(FLOAT n, vecN b)
{
    vecN result = {malloc(b.size * sizeof(FLOAT)), b.size};

    for (SIZE i = 0; i < b.size; i++)
    {
        result.data[i] = n * b.data[i];
    }

    return result;
}

vecN vecNDot(vecN a, vecN b)
{
    vecN result = {malloc(a.size * sizeof(FLOAT)), a.size};

    for (SIZE i = 0; i < a.size; i++)
    {
        result.data[i] = a.data[i] * b.data[i];
    }

    return result;
}

void vecNFree(vecN *v)
{
    if (v && v->data && v->size > 0)
    {
        free(v->data);

        v->data = NULL;
        v->size = 0;
    }
}