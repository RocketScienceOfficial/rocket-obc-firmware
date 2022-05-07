#pragma once

#include <stdio.h>

typedef struct bmp280_data
{
    int32_t temperature;
    int32_t pressure;
} bmp280_data_t;

void bmp280Init(int i2c, int sda, int scl);
void bmp280Read(bmp280_data_t *data);