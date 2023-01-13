#include "maths/matrix.h"
#include <stdlib.h>
#include <string.h>

mat2x2 mat2x2Add(mat2x2 a, mat2x2 b)
{
    mat2x2 result = {0};

    result.m0_0 = a.m0_0 + b.m0_0;
    result.m0_1 = a.m0_1 + b.m0_1;
    result.m1_0 = a.m1_0 + b.m1_0;
    result.m1_1 = a.m1_1 + b.m1_1;

    return result;
}

mat2x2 mat2x2Sub(mat2x2 a, mat2x2 b)
{
    mat2x2 result = {0};

    result.m0_0 = a.m0_0 - b.m0_0;
    result.m0_1 = a.m0_1 - b.m0_1;
    result.m1_0 = a.m1_0 - b.m1_0;
    result.m1_1 = a.m1_1 - b.m1_1;

    return result;
}

mat2x2 mat2x2MulNum(FLOAT n, mat2x2 m)
{
    mat2x2 result = {0};

    result.m0_0 = n * m.m0_0;
    result.m0_1 = n * m.m0_1;
    result.m1_0 = n * m.m1_0;
    result.m1_1 = n * m.m1_1;

    return result;
}

mat2x2 mat2x2Mul(mat2x2 a, mat2x2 b)
{
    mat2x2 result = {0};

    result.m0_0 = a.m0_0 * b.m0_0 + a.m0_1 * b.m1_0;
    result.m0_1 = a.m0_0 * b.m0_1 + a.m0_1 * b.m1_1;
    result.m1_0 = a.m1_0 * b.m0_0 + a.m1_1 * b.m1_0;
    result.m1_1 = a.m1_0 * b.m0_1 + a.m1_1 * b.m1_1;

    return result;
}

mat2x2 mat2x2Transpose(mat2x2 m)
{
    mat2x2 result = {0};

    result.m0_0 = m.m0_0;
    result.m0_1 = m.m1_0;
    result.m1_0 = m.m0_1;
    result.m1_1 = m.m1_1;

    return result;
}

mat3x3 mat3x3Add(mat3x3 a, mat3x3 b)
{
    mat3x3 result = {0};

    result.m0_0 = a.m0_0 + b.m0_0;
    result.m0_1 = a.m0_1 + b.m0_1;
    result.m0_2 = a.m0_2 + b.m0_2;
    result.m1_0 = a.m1_0 + b.m1_0;
    result.m1_1 = a.m1_1 + b.m1_1;
    result.m1_2 = a.m1_2 + b.m1_2;
    result.m2_0 = a.m2_0 + b.m2_0;
    result.m2_1 = a.m2_1 + b.m2_1;
    result.m2_2 = a.m2_2 + b.m2_2;

    return result;
}

mat3x3 mat3x3Sub(mat3x3 a, mat3x3 b)
{
    mat3x3 result = {0};

    result.m0_0 = a.m0_0 - b.m0_0;
    result.m0_1 = a.m0_1 - b.m0_1;
    result.m0_2 = a.m0_2 - b.m0_2;
    result.m1_0 = a.m1_0 - b.m1_0;
    result.m1_1 = a.m1_1 - b.m1_1;
    result.m1_2 = a.m1_2 - b.m1_2;
    result.m2_0 = a.m2_0 - b.m2_0;
    result.m2_1 = a.m2_1 - b.m2_1;
    result.m2_2 = a.m2_2 - b.m2_2;

    return result;
}

mat3x3 mat3x3MulNum(FLOAT n, mat3x3 m)
{
    mat3x3 result = {0};

    result.m0_0 = n * m.m0_0;
    result.m0_1 = n * m.m0_1;
    result.m0_2 = n * m.m0_2;
    result.m1_0 = n * m.m1_0;
    result.m1_1 = n * m.m1_1;
    result.m1_2 = n * m.m1_2;
    result.m2_0 = n * m.m2_0;
    result.m2_1 = n * m.m2_1;
    result.m2_2 = n * m.m2_2;

    return result;
}

mat3x3 mat3x3Mul(mat3x3 a, mat3x3 b)
{
    mat3x3 result = {0};

    result.m0_0 = a.m0_0 * b.m0_0 + a.m0_1 * b.m1_0 + a.m0_2 * b.m2_0;
    result.m0_1 = a.m0_0 * b.m0_1 + a.m0_1 * b.m1_1 + a.m0_2 * b.m2_1;
    result.m0_2 = a.m0_0 * b.m0_2 + a.m0_1 * b.m1_2 + a.m0_2 * b.m2_2;
    result.m1_0 = a.m1_0 * b.m0_0 + a.m1_1 * b.m1_0 + a.m1_2 * b.m2_0;
    result.m1_1 = a.m1_0 * b.m0_1 + a.m1_1 * b.m1_1 + a.m1_2 * b.m2_1;
    result.m1_2 = a.m1_0 * b.m0_2 + a.m1_1 * b.m1_2 + a.m1_2 * b.m2_2;
    result.m2_0 = a.m2_0 * b.m0_0 + a.m2_1 * b.m1_0 + a.m2_2 * b.m2_0;
    result.m2_1 = a.m2_0 * b.m0_1 + a.m2_1 * b.m1_1 + a.m2_2 * b.m2_1;
    result.m2_2 = a.m2_0 * b.m0_2 + a.m2_1 * b.m1_2 + a.m2_2 * b.m2_2;

    return result;
}

mat3x3 mat3x3Transpose(mat3x3 m)
{
    mat3x3 result = {0};

    result.m0_0 = m.m0_0;
    result.m0_1 = m.m1_0;
    result.m0_2 = m.m2_0;
    result.m1_0 = m.m0_1;
    result.m1_1 = m.m1_1;
    result.m1_2 = m.m2_1;
    result.m2_0 = m.m0_2;
    result.m2_1 = m.m1_2;
    result.m2_2 = m.m2_2;

    return result;
}

mat4x4 mat4x4Add(mat4x4 a, mat4x4 b)
{
    mat4x4 result = {0};

    result.m0_0 = a.m0_0 + b.m0_0;
    result.m0_1 = a.m0_1 + b.m0_1;
    result.m0_2 = a.m0_2 + b.m0_2;
    result.m0_3 = a.m0_3 + b.m0_3;
    result.m1_0 = a.m1_0 + b.m1_0;
    result.m1_1 = a.m1_1 + b.m1_1;
    result.m1_2 = a.m1_2 + b.m1_2;
    result.m1_3 = a.m1_3 + b.m1_3;
    result.m2_0 = a.m2_0 + b.m2_0;
    result.m2_1 = a.m2_1 + b.m2_1;
    result.m2_2 = a.m2_2 + b.m2_2;
    result.m2_3 = a.m2_3 + b.m2_3;
    result.m3_0 = a.m3_0 + b.m3_0;
    result.m3_1 = a.m3_1 + b.m3_1;
    result.m3_2 = a.m3_2 + b.m3_2;
    result.m3_3 = a.m3_3 + b.m3_3;

    return result;
}

mat4x4 mat4x4Sub(mat4x4 a, mat4x4 b)
{
    mat4x4 result = {0};

    result.m0_0 = a.m0_0 - b.m0_0;
    result.m0_1 = a.m0_1 - b.m0_1;
    result.m0_2 = a.m0_2 - b.m0_2;
    result.m0_3 = a.m0_3 - b.m0_3;
    result.m1_0 = a.m1_0 - b.m1_0;
    result.m1_1 = a.m1_1 - b.m1_1;
    result.m1_2 = a.m1_2 - b.m1_2;
    result.m1_3 = a.m1_3 - b.m1_3;
    result.m2_0 = a.m2_0 - b.m2_0;
    result.m2_1 = a.m2_1 - b.m2_1;
    result.m2_2 = a.m2_2 - b.m2_2;
    result.m2_3 = a.m2_3 - b.m2_3;
    result.m3_0 = a.m3_0 - b.m3_0;
    result.m3_1 = a.m3_1 - b.m3_1;
    result.m3_2 = a.m3_2 - b.m3_2;
    result.m3_3 = a.m3_3 - b.m3_3;

    return result;
}

mat4x4 mat4x4MulNum(FLOAT n, mat4x4 m)
{
    mat4x4 result = {0};

    result.m0_0 = n * m.m0_0;
    result.m0_1 = n * m.m0_1;
    result.m0_2 = n * m.m0_2;
    result.m0_3 = n * m.m0_3;
    result.m1_0 = n * m.m1_0;
    result.m1_1 = n * m.m1_1;
    result.m1_2 = n * m.m1_2;
    result.m1_3 = n * m.m1_3;
    result.m2_0 = n * m.m2_0;
    result.m2_1 = n * m.m2_1;
    result.m2_2 = n * m.m2_2;
    result.m2_3 = n * m.m2_3;
    result.m3_0 = n * m.m3_0;
    result.m3_1 = n * m.m3_1;
    result.m3_2 = n * m.m3_2;
    result.m3_3 = n * m.m3_3;

    return result;
}

mat4x4 mat4x4Mul(mat4x4 a, mat4x4 b)
{
    mat4x4 result = {0};

    result.m0_0 = a.m0_0 * b.m0_0 + a.m0_1 * b.m1_0 + a.m0_2 * b.m2_0 + a.m0_3 * b.m3_0;
    result.m0_1 = a.m0_0 * b.m0_1 + a.m0_1 * b.m1_1 + a.m0_2 * b.m2_1 + a.m0_3 * b.m3_1;
    result.m0_2 = a.m0_0 * b.m0_2 + a.m0_1 * b.m1_2 + a.m0_2 * b.m2_2 + a.m0_3 * b.m3_2;
    result.m0_3 = a.m0_0 * b.m0_3 + a.m0_1 * b.m1_3 + a.m0_2 * b.m2_3 + a.m0_3 * b.m3_3;
    result.m1_0 = a.m1_0 * b.m0_0 + a.m1_1 * b.m1_0 + a.m1_2 * b.m2_0 + a.m1_3 * b.m3_0;
    result.m1_1 = a.m1_0 * b.m0_1 + a.m1_1 * b.m1_1 + a.m1_2 * b.m2_1 + a.m1_3 * b.m3_1;
    result.m1_2 = a.m1_0 * b.m0_2 + a.m1_1 * b.m1_2 + a.m1_2 * b.m2_2 + a.m1_3 * b.m3_2;
    result.m1_3 = a.m1_0 * b.m0_3 + a.m1_1 * b.m1_3 + a.m1_2 * b.m2_3 + a.m1_3 * b.m3_3;
    result.m2_0 = a.m2_0 * b.m0_0 + a.m2_1 * b.m1_0 + a.m2_2 * b.m2_0 + a.m2_3 * b.m3_0;
    result.m2_1 = a.m2_0 * b.m0_1 + a.m2_1 * b.m1_1 + a.m2_2 * b.m2_1 + a.m2_3 * b.m3_1;
    result.m2_2 = a.m2_0 * b.m0_2 + a.m2_1 * b.m1_2 + a.m2_2 * b.m2_2 + a.m2_3 * b.m3_2;
    result.m2_3 = a.m2_0 * b.m0_3 + a.m2_1 * b.m1_3 + a.m2_2 * b.m2_3 + a.m2_3 * b.m3_3;
    result.m3_0 = a.m3_0 * b.m0_0 + a.m3_1 * b.m1_0 + a.m3_2 * b.m2_0 + a.m3_3 * b.m3_0;
    result.m3_1 = a.m3_0 * b.m0_1 + a.m3_1 * b.m1_1 + a.m3_2 * b.m2_1 + a.m3_3 * b.m3_1;
    result.m3_2 = a.m3_0 * b.m0_2 + a.m3_1 * b.m1_2 + a.m3_2 * b.m2_2 + a.m3_3 * b.m3_2;
    result.m3_3 = a.m3_0 * b.m0_3 + a.m3_1 * b.m1_3 + a.m3_2 * b.m2_3 + a.m3_3 * b.m3_3;

    return result;
}

mat4x4 mat4x4Transpose(mat4x4 m)
{
    mat4x4 result = {0};

    result.m0_0 = m.m0_0;
    result.m0_1 = m.m1_0;
    result.m0_2 = m.m2_0;
    result.m0_3 = m.m3_0;
    result.m1_0 = m.m0_1;
    result.m1_1 = m.m1_1;
    result.m1_2 = m.m2_1;
    result.m1_3 = m.m3_1;
    result.m2_0 = m.m0_2;
    result.m2_1 = m.m1_2;
    result.m2_2 = m.m2_2;
    result.m2_3 = m.m3_2;
    result.m3_0 = m.m0_3;
    result.m3_1 = m.m1_3;
    result.m3_2 = m.m2_3;
    result.m3_3 = m.m3_3;

    return result;
}

matMxN matMxNCreate(SIZE m, SIZE n)
{
    FLOAT **data = (FLOAT **)calloc(m, sizeof(FLOAT *));

    for (SIZE i = 0; i < m; i++)
    {
        data[i] = (FLOAT *)calloc(n, sizeof(FLOAT));
    }

    return (matMxN){data, m, n};
}

VOID matMxNFree(matMxN m)
{
    if (m.data && m.rows > 0 && m.cols > 0)
    {
        for (SIZE i = 0; i < m.rows; i++)
        {
            free(m.data[i]);
        }

        free(m.data);

        m.data = NULL;
        m.rows = 0;
        m.cols = 0;
    }
}

matMxN matMxNAdd(matMxN a, matMxN b)
{
    matMxN result = matMxNCreate(a.rows, b.cols);

    for (SIZE i = 0; i < a.rows; i++)
    {
        for (SIZE j = 0; j < a.cols; j++)
        {
            result.data[i][j] = a.data[i][j] + b.data[i][j];
        }
    }

    return result;
}

matMxN matMxNSub(matMxN a, matMxN b)
{
    matMxN result = matMxNCreate(a.rows, b.cols);

    for (SIZE i = 0; i < a.rows; i++)
    {
        for (SIZE j = 0; j < a.cols; j++)
        {
            result.data[i][j] = a.data[i][j] - b.data[i][j];
        }
    }

    return result;
}

matMxN matMxNMulNum(FLOAT n, matMxN m)
{
    matMxN result = matMxNCreate(m.rows, m.cols);

    for (SIZE i = 0; i < m.rows; i++)
    {
        for (SIZE j = 0; j < m.cols; j++)
        {
            result.data[i][j] = n * m.data[i][j];
        }
    }

    return result;
}

matMxN matMxNMul(matMxN a, matMxN b)
{
    matMxN result = matMxNCreate(a.rows, b.cols);

    for (SIZE i = 0; i < a.rows; i++)
    {
        for (SIZE j = 0; j < b.cols; j++)
        {
            for (SIZE k = 0; k < a.cols; k++)
            {
                result.data[i][j] += a.data[i][k] * b.data[j][k];
            }
        }
    }

    return result;
}

matMxN matMxNTranspose(matMxN m)
{
    matMxN result = matMxNCreate(m.cols, m.rows);

    for (SIZE i = 0; i < m.rows; i++)
    {
        for (SIZE j = 0; j < m.cols; j++)
        {
            result.data[j][i] = m.data[i][j];
        }
    }

    return result;
}

matMxN vecToMat(vecN v)
{
    matMxN result = matMxNCreate(v.size, 1);

    memcpy(&result.data[0], v.data, v.size * sizeof(FLOAT));

    return result;
}

VOID vecToMatRef(matMxN *matRef, vecN v)
{
    memcpy(&matRef->data[0], v.data, v.size * sizeof(FLOAT));

    matRef->rows = v.size;
}