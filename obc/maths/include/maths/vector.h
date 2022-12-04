#pragma once

#include <obc/api.h>

typedef struct vec2
{
    FLOAT x, y;
} vec2;

typedef struct vec3
{
    FLOAT x, y, z;
} vec3;

typedef struct vec4
{
    FLOAT x, y, z, w;
} vec4;

typedef struct vecN
{
    FLOAT *data;
    SIZE size;
} vecN;

vec2 vec2Add(vec2 a, vec2 b);
vec2 vec2Sub(vec2 a, vec2 b);
vec2 vec2MulNum(FLOAT n, vec2 b);
vec2 vec2Dot(vec2 a, vec2 b);

vec3 vec3Add(vec3 a, vec3 b);
vec3 vec3Sub(vec3 a, vec3 b);
vec3 vec3MulNum(FLOAT n, vec3 b);
vec3 vec3Dot(vec3 a, vec3 b);

vec4 vec4Add(vec4 a, vec4 b);
vec4 vec4Sub(vec4 a, vec4 b);
vec4 vec4MulNum(FLOAT n, vec4 b);
vec4 vec4Dot(vec4 a, vec4 b);

vecN vecNAdd(vecN a, vecN b);
vecN vecNSub(vecN a, vecN b);
vecN vecNMulNum(FLOAT n, vecN b);
vecN vecNDot(vecN a, vecN b);
void vecNFree(vecN *v);