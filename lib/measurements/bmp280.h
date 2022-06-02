#pragma once

#include <stdio.h>

#define PRESSURE_TEMPERATURE_CONSTANT 44330.769
#define PRESSURE_GAS_CONSTANT 0.19026
#define SEA_LEVEL_PRESSURE 101325.0

typedef struct bmp280_data
{
    float temperature;
    int pressure;
} bmp280_data_t;

int bmp280Init(int i2c, int sda, int scl);
int bmp280Check();
void bmp280Read(bmp280_data_t *data);
float bmp280GetAltitude(bmp280_data_t* data);