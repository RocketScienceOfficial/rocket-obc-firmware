#pragma once

#include <obc/api.h>

/**
 * @brief Vector 2D.
*/
typedef struct vec2
{
    FLOAT x, y;
} vec2;

/**
 * @brief Vector 3D.
*/
typedef struct vec3
{
    FLOAT x, y, z;
} vec3;

/**
 * @brief Vector 4D.
*/
typedef struct vec4
{
    FLOAT x, y, z, w;
} vec4;

/**
 * @brief Vector of N dimensions.
*/
typedef struct vecN
{
    FLOAT *data;
    SIZE size;
} vecN;

vec2 vec2Add(vec2 a, vec2 b);
vec2 vec2Sub(vec2 a, vec2 b);
FLOAT vec2Magnitude(vec2 v);
vec2 vec2Normalize(vec2 v);
vec2 vec2MulNum(FLOAT n, vec2 b);
vec2 vec2Dot(vec2 a, vec2 b);
vecN vec2ToVecN(vec2 v);
VOID vec2ToVecNRef(vecN* vRef, vec2 v);

vec3 vec3Add(vec3 a, vec3 b);
vec3 vec3Sub(vec3 a, vec3 b);
FLOAT vec3Magnitude(vec3 v);
vec3 vec3Normalize(vec3 v);
vec3 vec3MulNum(FLOAT n, vec3 b);
vec3 vec3Dot(vec3 a, vec3 b);
vecN vec3ToVecN(vec3 v);
VOID vec3ToVecNRef(vecN* vRef, vec3 v);

vec4 vec4Add(vec4 a, vec4 b);
vec4 vec4Sub(vec4 a, vec4 b);
FLOAT vec4Magnitude(vec4 v);
vec4 vec4Normalize(vec4 v);
vec4 vec4MulNum(FLOAT n, vec4 b);
vec4 vec4Dot(vec4 a, vec4 b);
vecN vec4ToVecN(vec4 v);
VOID vec4ToVecNRef(vecN* vRef, vec4 v);

vecN vecNCreate(SIZE size);
VOID vecNFree(vecN v);
vecN vecNAdd(vecN a, vecN b);
vecN vecNSub(vecN a, vecN b);
FLOAT vecNMagnitude(vecN v);
vecN vecNNormalize(vecN v);
vecN vecNMulNum(FLOAT n, vecN b);
vecN vecNDot(vecN a, vecN b);