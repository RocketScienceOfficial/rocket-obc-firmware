#pragma once

#include <obc/api.h>
#include "vector.h"

/**
 * @brief Matrix 2x2.
 */
typedef union mat2x2
{
    struct
    {
        FLOAT data[2][2];
    };

    struct
    {
        FLOAT m0_0, m0_1;
        FLOAT m1_0, m1_1;
    };
} mat2x2;

/**
 * @brief Matrix 3x3.
 */
typedef union mat3x3
{
    struct
    {
        FLOAT data[3][3];
    };

    struct
    {
        FLOAT m0_0, m0_1, m0_2;
        FLOAT m1_0, m1_1, m1_2;
        FLOAT m2_0, m2_1, m2_2;
    };

} mat3x3;

/**
 * @brief Matrix 4x4.
 */
typedef union mat4x4
{
    struct
    {
        FLOAT data[4][4];
    };

    struct
    {
        FLOAT m0_0, m0_1, m0_2, m0_3;
        FLOAT m1_0, m1_1, m1_2, m1_3;
        FLOAT m2_0, m2_1, m2_2, m2_3;
        FLOAT m3_0, m3_1, m3_2, m3_3;
    };
} mat4x4;

/**
 * @brief Matrix of M by N dimensions.
 */
typedef struct matMxN
{
    FLOAT **data;
    SIZE rows;
    SIZE cols;
} matMxN;

mat2x2 mat2x2Add(mat2x2 a, mat2x2 b);
mat2x2 mat2x2Sub(mat2x2 a, mat2x2 b);
mat2x2 mat2x2MulNum(FLOAT n, mat2x2 m);
mat2x2 mat2x2Mul(mat2x2 a, mat2x2 b);
mat2x2 mat2x2Transpose(mat2x2 m);

mat3x3 mat3x3Add(mat3x3 a, mat3x3 b);
mat3x3 mat3x3Sub(mat3x3 a, mat3x3 b);
mat3x3 mat3x3MulNum(FLOAT n, mat3x3 m);
mat3x3 mat3x3Mul(mat3x3 a, mat3x3 b);
mat3x3 mat3x3Transpose(mat3x3 m);

mat4x4 mat4x4Add(mat4x4 a, mat4x4 b);
mat4x4 mat4x4Sub(mat4x4 a, mat4x4 b);
mat4x4 mat4x4MulNum(FLOAT n, mat4x4 m);
mat4x4 mat4x4Mul(mat4x4 a, mat4x4 b);
mat4x4 mat4x4Transpose(mat4x4 m);

matMxN matMxNCreate(SIZE m, SIZE n);
VOID matMxNFree(matMxN m);
matMxN matMxNAdd(matMxN a, matMxN b);
matMxN matMxNSub(matMxN a, matMxN b);
matMxN matMxNMulNum(FLOAT n, matMxN m);
matMxN matMxNMul(matMxN a, matMxN b);
matMxN matMxNTranspose(matMxN m);
matMxN vecToMat(vecN v);
VOID vecToMatRef(matMxN* matRef, vecN v);