#include "bmp280.h"
#include "logger.h"
#include "hardware/i2c.h"
#include "pico/stdlib.h"
#include <math.h>

#define BMP280_ADDR _u(0x76)

#define BMP280_REG_CONFIG _u(0xF5)
#define BMP280_REG_CTRL_MEAS _u(0xF4)
#define BMP280_REG_RESET _u(0xE0)

#define BMP280_REG_TEMP_XLSB _u(0xFC)
#define BMP280_REG_TEMP_LSB _u(0xFB)
#define BMP280_REG_TEMP_MSB _u(0xFA)

#define BMP280_REG_PRESSURE_XLSB _u(0xF9)
#define BMP280_REG_PRESSURE_LSB _u(0xF8)
#define BMP280_REG_PRESSURE_MSB _u(0xF7)

#define BMP280_REG_DIG_T1_LSB _u(0x88)
#define BMP280_REG_DIG_T1_MSB _u(0x89)
#define BMP280_REG_DIG_T2_LSB _u(0x8A)
#define BMP280_REG_DIG_T2_MSB _u(0x8B)
#define BMP280_REG_DIG_T3_LSB _u(0x8C)
#define BMP280_REG_DIG_T3_MSB _u(0x8D)
#define BMP280_REG_DIG_P1_LSB _u(0x8E)
#define BMP280_REG_DIG_P1_MSB _u(0x8F)
#define BMP280_REG_DIG_P2_LSB _u(0x90)
#define BMP280_REG_DIG_P2_MSB _u(0x91)
#define BMP280_REG_DIG_P3_LSB _u(0x92)
#define BMP280_REG_DIG_P3_MSB _u(0x93)
#define BMP280_REG_DIG_P4_LSB _u(0x94)
#define BMP280_REG_DIG_P4_MSB _u(0x95)
#define BMP280_REG_DIG_P5_LSB _u(0x96)
#define BMP280_REG_DIG_P5_MSB _u(0x97)
#define BMP280_REG_DIG_P6_LSB _u(0x98)
#define BMP280_REG_DIG_P6_MSB _u(0x99)
#define BMP280_REG_DIG_P7_LSB _u(0x9A)
#define BMP280_REG_DIG_P7_MSB _u(0x9B)
#define BMP280_REG_DIG_P8_LSB _u(0x9C)
#define BMP280_REG_DIG_P8_MSB _u(0x9D)
#define BMP280_REG_DIG_P9_LSB _u(0x9E)
#define BMP280_REG_DIG_P9_MSB _u(0x9F)

#define BMP280_NUM_CALIB_PARAMS 24

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

static int s_i2c;

static i2c_inst_t *getI2C()
{
    return (s_i2c == 0 ? i2c0 : i2c1);
}

void bmp280InitSensor()
{
    uint8_t buf[2];

    const uint8_t reg_config_val = ((0x04 << 5) | (0x05 << 2)) & 0xFC;

    buf[0] = BMP280_REG_CONFIG;
    buf[1] = reg_config_val;
    i2c_write_blocking(getI2C(), BMP280_ADDR, buf, 2, false);

    const uint8_t reg_ctrl_meas_val = (0x01 << 5) | (0x03 << 2) | (0x03);
    buf[0] = BMP280_REG_CTRL_MEAS;
    buf[1] = reg_ctrl_meas_val;
    i2c_write_blocking(getI2C(), BMP280_ADDR, buf, 2, false);
}

void bmp280ReadRaw(int32_t *temp, int32_t *pressure)
{
    uint8_t buf[6];
    uint8_t reg = BMP280_REG_PRESSURE_MSB;
    i2c_write_blocking(getI2C(), BMP280_ADDR, &reg, 1, true);
    i2c_read_blocking(getI2C(), BMP280_ADDR, buf, 6, false);

    *pressure = (buf[0] << 12) | (buf[1] << 4) | (buf[2] >> 4);
    *temp = (buf[3] << 12) | (buf[4] << 4) | (buf[5] >> 4);
}

void bmp280Reset()
{
    uint8_t buf[2] = {BMP280_REG_RESET, 0xB6};
    i2c_write_blocking(getI2C(), BMP280_ADDR, buf, 2, false);
}

int32_t bmp280Convert(int32_t temp, struct bmp280_calib_param *params)
{
    int32_t var1, var2;
    var1 = ((((temp >> 3) - ((int32_t)params->dig_t1 << 1))) * ((int32_t)params->dig_t2)) >> 11;
    var2 = (((((temp >> 4) - ((int32_t)params->dig_t1)) * ((temp >> 4) - ((int32_t)params->dig_t1))) >> 12) * ((int32_t)params->dig_t3)) >> 14;
    return var1 + var2;
}

float bmp280ConvertTemp(int32_t temp, struct bmp280_calib_param *params)
{
    int32_t t_fine = bmp280Convert(temp, params);
    int32_t final = (t_fine * 5 + 128) >> 8;
    float result = final / 100.0f;

    return result;
}

int bmp280ConvertPressure(int32_t pressure, int32_t temp, struct bmp280_calib_param *params)
{
    int32_t t_fine = bmp280Convert(temp, params);

    int32_t var1, var2;
    uint32_t converted = 0.0;
    var1 = (((int32_t)t_fine) >> 1) - (int32_t)64000;
    var2 = (((var1 >> 2) * (var1 >> 2)) >> 11) * ((int32_t)params->dig_p6);
    var2 += ((var1 * ((int32_t)params->dig_p5)) << 1);
    var2 = (var2 >> 2) + (((int32_t)params->dig_p4) << 16);
    var1 = (((params->dig_p3 * (((var1 >> 2) * (var1 >> 2)) >> 13)) >> 3) + ((((int32_t)params->dig_p2) * var1) >> 1)) >> 18;
    var1 = ((((32768 + var1)) * ((int32_t)params->dig_p1)) >> 15);
    if (var1 == 0)
    {
        return 0;
    }
    converted = (((uint32_t)(((int32_t)1048576) - pressure) - (var2 >> 12))) * 3125;
    if (converted < 0x80000000)
    {
        converted = (converted << 1) / ((uint32_t)var1);
    }
    else
    {
        converted = (converted / (uint32_t)var1) * 2;
    }
    var1 = (((int32_t)params->dig_p9) * ((int32_t)(((converted >> 3) * (converted >> 3)) >> 13))) >> 12;
    var2 = (((int32_t)(converted >> 2)) * ((int32_t)params->dig_p8)) >> 13;
    converted = (uint32_t)((int32_t)converted + ((var1 + var2 + params->dig_p7) >> 4));

    return converted;
}

void bmp280GetCalibParams(struct bmp280_calib_param *params)
{
    uint8_t buf[BMP280_NUM_CALIB_PARAMS] = {0};
    uint8_t reg = BMP280_REG_DIG_T1_LSB;
    i2c_write_blocking(getI2C(), BMP280_ADDR, &reg, 1, true);
    i2c_read_blocking(getI2C(), BMP280_ADDR, buf, BMP280_NUM_CALIB_PARAMS, false);

    params->dig_t1 = (uint16_t)(buf[1] << 8) | buf[0];
    params->dig_t2 = (int16_t)(buf[3] << 8) | buf[2];
    params->dig_t3 = (int16_t)(buf[5] << 8) | buf[4];

    params->dig_p1 = (uint16_t)(buf[7] << 8) | buf[6];
    params->dig_p2 = (int16_t)(buf[9] << 8) | buf[8];
    params->dig_p3 = (int16_t)(buf[11] << 8) | buf[10];
    params->dig_p4 = (int16_t)(buf[13] << 8) | buf[12];
    params->dig_p5 = (int16_t)(buf[15] << 8) | buf[14];
    params->dig_p6 = (int16_t)(buf[17] << 8) | buf[16];
    params->dig_p7 = (int16_t)(buf[19] << 8) | buf[18];
    params->dig_p8 = (int16_t)(buf[21] << 8) | buf[20];
    params->dig_p9 = (int16_t)(buf[23] << 8) | buf[22];
}

int bmp280Init(int i2c, int sda, int scl)
{
    MY_LOG_CORE_INFO("Initializing BMP280...");

    s_i2c = i2c;

    i2c_init(getI2C(), 100 * 1000);
    gpio_set_function(sda, GPIO_FUNC_I2C);
    gpio_set_function(scl, GPIO_FUNC_I2C);
    gpio_pull_up(sda);
    gpio_pull_up(scl);

    if (!bmp280Check())
    {
        return 0;
    }

    bmp280InitSensor();

    bmp280GetCalibParams(&params);

    MY_LOG_CORE_INFO("BMP280 Initialized!");

    return 1;
}

int bmp280Check()
{
    uint8_t data;
    int ret = i2c_read_blocking(getI2C(), BMP280_ADDR, &data, 1, false);

    return ret < 0 ? 0 : 1;
}

void bmp280Read(bmp280_data_t *data)
{
    MY_LOG_CORE_INFO("Reading BMP280...");

    int32_t raw_temperature;
    int32_t raw_pressure;

    bmp280ReadRaw(&raw_temperature, &raw_pressure);

    data->temperature = bmp280ConvertTemp(raw_temperature, &params);
    data->pressure = bmp280ConvertPressure(raw_pressure, raw_temperature, &params);

    MY_LOG_CORE_INFO("Succesffully read BMP280!");
}

float bmp280GetAltitude(bmp280_data_t *data)
{
    int pressure = data->pressure;
    float altitude = PRESSURE_TEMPERATURE_CONSTANT * (1.0 - pow(pressure / SEA_LEVEL_PRESSURE, PRESSURE_GAS_CONSTANT));

    return altitude;
}