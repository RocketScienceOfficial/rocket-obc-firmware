#pragma once

typedef struct config_data
{
    int bmp280_i2c;
    int bmp280_i2c_sda;
    int bmp280_i2c_scl;
    int mpu6050_i2c;
    int mpu6050_i2c_sda;
    int mpu6050_i2c_scl;
    int sd_miso;
    int sd_mosi;
    int sd_sck;
    int sd_sc;
    int sd_freq;
} config_data_t;

extern config_data_t *g_config;

void config_init(const char* yaml, config_data_t *config);