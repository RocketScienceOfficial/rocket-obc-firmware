#pragma once

#include <stdio.h>

#define PRESSURE_TEMPERATURE_CONSTANT 44330.769
#define PRESSURE_GAS_CONSTANT 0.19026
#define SEA_LEVEL_PRESSURE 101325.0

typedef struct barometer_data
{
    float temperature;
    int pressure;
} barometer_data_t;

int barometerInit(int i2c, int sda, int scl);
int barometerCheck();
void barometerRead(barometer_data_t *data);
float barometerGetAltitude(barometer_data_t* data);