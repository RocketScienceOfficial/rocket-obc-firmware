#include "maths/matrix.h"
#include <stdlib.h>

mat2x2 mat2x2Add(mat2x2 a, mat2x2 b)
{
    mat2x2 result;

    result.m0_0 = a.m0_0 + b.m0_0;
    result.m0_1 = a.m0_1 + b.m0_1;
    result.m1_0 = a.m1_0 + b.m1_0;
    result.m1_1 = a.m1_1 + b.m1_1;

    return result;
}

mat2x2 mat2x2Sub(mat2x2 a, mat2x2 b)
{
    mat2x2 result;

    result.m0_0 = a.m0_0 - b.m0_0;
    result.m0_1 = a.m0_1 - b.m0_1;
    result.m1_0 = a.m1_0 - b.m1_0;
    result.m1_1 = a.m1_1 - b.m1_1;

    return result;
}

mat2x2 mat2x2MulNum(FLOAT n, mat2x2 m)
{
    mat2x2 result;

    result.m0_0 = n * m.m0_0;
    result.m0_1 = n * m.m0_1;
    result.m1_0 = n * m.m1_0;
    result.m1_1 = n * m.m1_1;

    return result;
}

mat2x2 mat2x2Mul(mat2x2 a, mat2x2 b)
{
    mat2x2 result;

    result.m0_0 = a.m0_0 * b.m0_0 + a.m0_1 * b.m1_0;
    result.m0_1 = a.m0_0 * b.m0_1 + a.m0_1 * b.m1_1;
    result.m1_0 = a.m1_0 * b.m0_0 + a.m1_1 * b.m1_0;
    result.m1_1 = a.m1_0 * b.m0_1 + a.m1_1 * b.m1_1;

    return result;
}

mat3x3 mat3x3Add(mat3x3 a, mat3x3 b)
{
    mat3x3 result;

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
    mat3x3 result;

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
    mat3x3 result;

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
    mat3x3 result;

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

mat4x4 mat4x4Add(mat4x4 a, mat4x4 b)
{
    mat4x4 result;

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
    mat4x4 result;

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
    mat4x4 result;

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
    mat4x4 result;

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

matNxM matNxMAdd(matNxM a, matNxM b)
{
    matNxM result = {(FLOAT *)malloc(a.rows * a.cols * sizeof(FLOAT)), a.rows, a.cols};

    for (SIZE i = 0; i < a.rows; i++)
    {
        for (SIZE j = 0; j < a.cols; j++)
        {
            SIZE index = i * a.cols + j;

            result.data[index] = a.data[index] + b.data[index];
        }
    }

    return result;
}

matNxM matNxMSub(matNxM a, matNxM b)
{
    matNxM result = {(FLOAT *)malloc(a.rows * a.cols * sizeof(FLOAT)), a.rows, a.cols};

    for (SIZE i = 0; i < a.rows; i++)
    {
        for (SIZE j = 0; j < a.cols; j++)
        {
            SIZE index = i * a.cols + j;

            result.data[index] = a.data[index] - b.data[index];
        }
    }

    return result;
}

matNxM matNxMMulNum(FLOAT n, matNxM m)
{
    matNxM result = {(FLOAT *)malloc(m.rows * m.cols * sizeof(FLOAT)), m.rows, m.cols};

    for (SIZE i = 0; i < m.rows; i++)
    {
        for (SIZE j = 0; j < m.cols; j++)
        {
            SIZE index = i * m.cols + j;

            result.data[index] = n * m.data[index];
        }
    }

    return result;
}

matNxM matNxMMul(matNxM a, matNxM b)
{
    matNxM result = {(FLOAT *)malloc(a.rows * b.cols * sizeof(FLOAT)), a.rows, b.cols};

    for (SIZE i = 0; i < a.rows; i++)
    {
        for (SIZE j = 0; j < b.cols; j++)
        {
            for (SIZE k = 0; k < a.cols; k++)
            {
                SIZE index = i * b.cols + j;

                result.data[index] += a.data[i * a.cols + k] * b.data[k * b.cols + j];
            }
        }
    }

    return result;
}

matNxM vecToMat(vecN v)
{
    matNxM result = {(FLOAT *)malloc(1 * v.size * sizeof(FLOAT)), v.size, 1};

    return result;
}

void matNxMFree(matNxM *m)
{
    if (m && m->data && m->rows > 0 && m->cols > 0)
    {
        free(m->data);

        m->data = NULL;
        m->rows = 0;
        m->cols = 0;
    }
}