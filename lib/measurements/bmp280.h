#pragma once

#include <stdio.h>

struct bmp280_calib_param
{
    uint16_t dig_t1;
    int16_t dig_t2;
    int16_t dig_t3;

    uint16_t dig_p1;
    int16_t dig_p2;
    int16_t dig_p3;
    int16_t dig_p4;
    int16_t dig_p5;
    int16_t dig_p6;
    int16_t dig_p7;
    int16_t dig_p8;
    int16_t dig_p9;
} params;

typedef struct bmp280_data
{
    int32_t temperature;
    int32_t pressure;
} bmp280_data_t;

void bmp280Init(int i2c, int sda, int scl);
void bmp280Read(bmp280_data_t *data);