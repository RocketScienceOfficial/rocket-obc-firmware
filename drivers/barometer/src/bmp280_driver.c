#include "drivers/barometer/bmp280_driver.h"
#include "pico/stdlib.h"

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

void bmp280InitSensor(BMP280Config *config)
{
    BYTE buf[2];

    const BYTE reg_config_val = ((0x04 << 5) | (0x05 << 2)) & 0xFC;

    buf[0] = BMP280_REG_CONFIG;
    buf[1] = reg_config_val;
    i2cWriteBlocking(config->i2c, BMP280_ADDR, buf, 2, FALSE);

    const BYTE reg_ctrl_meas_val = (0x01 << 5) | (0x03 << 2) | (0x03);

    buf[0] = BMP280_REG_CTRL_MEAS;
    buf[1] = reg_ctrl_meas_val;
    i2cWriteBlocking(config->i2c, BMP280_ADDR, buf, 2, FALSE);
}

void bmp280ReadRaw(BMP280Config *config, INT32 *temp, INT32 *pressure)
{
    BYTE buf[6];
    BYTE reg = BMP280_REG_PRESSURE_MSB;

    i2cWriteBlocking(config->i2c, BMP280_ADDR, &reg, 1, TRUE);
    i2cReadBlocking(config->i2c, BMP280_ADDR, buf, 6, FALSE);

    *pressure = (buf[0] << 12) | (buf[1] << 4) | (buf[2] >> 4);
    *temp = (buf[3] << 12) | (buf[4] << 4) | (buf[5] >> 4);
}

void bmp280Reset(BMP280Config *config)
{
    BYTE buf[2] = {BMP280_REG_RESET, 0xB6};

    i2cWriteBlocking(config->i2c, BMP280_ADDR, buf, 2, FALSE);
}

INT32 bmp280Convert(INT32 temp, BMP280Config *config)
{
    INT32 var1, var2;
    BMP280CalibParams *params = &config->calibParams;

    var1 = ((((temp >> 3) - ((INT32)params->dig_t1 << 1))) * ((INT32)params->dig_t2)) >> 11;
    var2 = (((((temp >> 4) - ((INT32)params->dig_t1)) * ((temp >> 4) - ((INT32)params->dig_t1))) >> 12) * ((INT32)params->dig_t3)) >> 14;

    return var1 + var2;
}

FLOAT bmp280ConvertTemp(INT32 temp, BMP280Config *config)
{
    INT32 t_fine = bmp280Convert(temp, config);
    INT32 final = (t_fine * 5 + 128) >> 8;
    FLOAT result = final / 100.0f;

    return result;
}

INT32 bmp280ConvertPressure(INT32 pressure, INT32 temp, BMP280Config *config)
{
    BMP280CalibParams *params = &config->calibParams;
    INT32 t_fine = bmp280Convert(temp, config);
    INT32 var1, var2;
    UINT32 converted = 0.0;

    var1 = (((INT32)t_fine) >> 1) - (INT32)64000;
    var2 = (((var1 >> 2) * (var1 >> 2)) >> 11) * ((INT32)params->dig_p6);
    var2 += ((var1 * ((INT32)params->dig_p5)) << 1);
    var2 = (var2 >> 2) + (((INT32)params->dig_p4) << 16);
    var1 = (((params->dig_p3 * (((var1 >> 2) * (var1 >> 2)) >> 13)) >> 3) + ((((INT32)params->dig_p2) * var1) >> 1)) >> 18;
    var1 = ((((32768 + var1)) * ((INT32)params->dig_p1)) >> 15);

    if (var1 == 0)
    {
        return 0;
    }

    converted = (((UINT32)(((INT32)1048576) - pressure) - (var2 >> 12))) * 3125;

    if (converted < 0x80000000)
    {
        converted = (converted << 1) / ((UINT32)var1);
    }
    else
    {
        converted = (converted / (UINT32)var1) * 2;
    }

    var1 = (((INT32)params->dig_p9) * ((INT32)(((converted >> 3) * (converted >> 3)) >> 13))) >> 12;
    var2 = (((INT32)(converted >> 2)) * ((INT32)params->dig_p8)) >> 13;
    converted = (UINT32)((INT32)converted + ((var1 + var2 + params->dig_p7) >> 4));

    return (INT32)converted;
}

void bmp280GetCalibParams(BMP280Config *config)
{
    BMP280CalibParams *params = &config->calibParams;
    BYTE buf[BMP280_NUM_CALIB_PARAMS] = {0};
    BYTE reg = BMP280_REG_DIG_T1_LSB;

    i2cWriteBlocking(config->i2c, BMP280_ADDR, &reg, 1, TRUE);
    i2cReadBlocking(config->i2c, BMP280_ADDR, buf, BMP280_NUM_CALIB_PARAMS, FALSE);

    params->dig_t1 = (UINT16)(buf[1] << 8) | buf[0];
    params->dig_t2 = (INT16)(buf[3] << 8) | buf[2];
    params->dig_t3 = (INT16)(buf[5] << 8) | buf[4];

    params->dig_p1 = (UINT16)(buf[7] << 8) | buf[6];
    params->dig_p2 = (INT16)(buf[9] << 8) | buf[8];
    params->dig_p3 = (INT16)(buf[11] << 8) | buf[10];
    params->dig_p4 = (INT16)(buf[13] << 8) | buf[12];
    params->dig_p5 = (INT16)(buf[15] << 8) | buf[14];
    params->dig_p6 = (INT16)(buf[17] << 8) | buf[16];
    params->dig_p7 = (INT16)(buf[19] << 8) | buf[18];
    params->dig_p8 = (INT16)(buf[21] << 8) | buf[20];
    params->dig_p9 = (INT16)(buf[23] << 8) | buf[22];
}

FUNCRESULT bmp280Init(BMP280Config *config, I2CInstance i2c, PinNumber sda, PinNumber scl)
{
    if (!config || !i2cCheckInstance(i2c) || !i2cCheckSDA(i2c, sda) || !i2cCheckSCL(i2c, scl))
    {
        return ERR_INVALIDARG;
    }

    config->i2c = i2c;

    if (FUNCFAILED(i2cInitPins(i2c, sda, scl)))
    {
        return ERR_FAIL;
    }

    BOOL result;

    if (FUNCFAILED(bmp280Check(config, &result)))
    {
        return ERR_UNEXPECTED;
    }

    if (!result)
    {
        return ERR_UNINITIALIZED;
    }

    bmp280InitSensor(config);
    bmp280GetCalibParams(config);

    return SUC_OK;
}

FUNCRESULT bmp280Check(BMP280Config *config, BOOL *result)
{
    if (!config || !result)
    {
        return ERR_INVALIDARG;
    }

    *result = i2cCheckDevice(config->i2c, BMP280_ADDR);

    return SUC_OK;
}

FUNCRESULT bmp280Read(BMP280Config *config, BMP280Data *data)
{
    if (!config || !data)
    {
        return ERR_INVALIDARG;
    }

    INT32 raw_temperature;
    INT32 raw_pressure;

    bmp280ReadRaw(config, &raw_temperature, &raw_pressure);

    data->temperature = bmp280ConvertTemp(raw_temperature, config);
    data->pressure = bmp280ConvertPressure(raw_pressure, raw_temperature, config);

    return SUC_OK;
}