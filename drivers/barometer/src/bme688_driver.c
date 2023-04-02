#include "drivers/barometer/bme688_driver.h"
#include "pico/stdlib.h"

#define STATUS 0x73
#define VARIANT_ID 0x70
#define RESET 0x60
#define CHIP_ID 0x50
#define CONFIG 0x75
#define CTRL_MEAS 0x74
#define CTRL_HUM 0x72
#define CTRL_GAS_1 0x71
#define CTRL_GAS_0 0x70
#define GAS_WAIT_SHARED 0x6E
#define GAS_WAIT_X 0x64
#define RES_HEAT_X 0x5A
#define IDAC_HEAT_X 0x50
#define SUB_MEAS_INDEX_0 0x1E
#define SUB_MEAS_INDEX_1 0x2F
#define SUB_MEAS_INDEX_2 0x40
#define MEAS_STATUS_0 0x1D
#define MEAS_STATUS_1 0x2E
#define MEAS_STATUS_2 0x3F
#define PRESS_MSB_0 0x1F
#define PRESS_MSB_1 0x30
#define PRESS_MSB_2 0x41
#define PRESS_LSB_0 0x20
#define PRESS_LSB_1 0x31
#define PRESS_LSB_2 0x42
#define PRESS_XLSB_0 0x21
#define PRESS_XLSB_1 0x32
#define PRESS_XLSB_2 0x43
#define TEMP_MSB_0 0x22
#define TEMP_MSB_1 0x33
#define TEMP_MSB_2 0x44
#define TEMP_LSB_0 0x23
#define TEMP_LSB_1 0x34
#define TEMP_LSB_2 0x45
#define TEMP_XLSB_0 0x24
#define TEMP_XLSB_1 0x35
#define TEMP_XLSB_2 0x46
#define HUM_MSB_0 0x25
#define HUM_MSB_1 0x36
#define HUM_MSB_2 0x47
#define HUM_LSB_0 0x26
#define HUM_LSB_1 0x37
#define HUM_LSB_2 0x48
#define GAS_R_MSB_0 0x2C
#define GAS_R_MSB_1 0x3D
#define GAS_R_MSB_2 0x4E
#define GAS_R_LSB_0 0x2D
#define GAS_R_LSB_1 0x3E
#define GAS_R_LSB_2 0x4F

#define SOFT_RESET_CMD 0xB6

#define CALIB_T1_LSB 0xE9
#define CALIB_T1_MSB 0xEA
#define CALIB_T2_LSB 0x8A
#define CALIB_T2_MSB 0x8B
#define CALIB_T3_LSB 0x8C
#define CALIB_P1_LSB 0x8E
#define CALIB_P1_MSB 0x8F
#define CALIB_P2_LSB 0x90
#define CALIB_P2_MSB 0x91
#define CALIB_P3_LSB 0x92
#define CALIB_P4_LSB 0x94
#define CALIB_P4_MSB 0x95
#define CALIB_P5_LSB 0x96
#define CALIB_P5_MSB 0x97
#define CALIB_P6_LSB 0x99
#define CALIB_P7_LSB 0x98
#define CALIB_P8_LSB 0x9C
#define CALIB_P8_MSB 0x9D
#define CALIB_P9_LSB 0x9E
#define CALIB_P9_MSB 0x9F
#define CALIB_P10_LSB 0xA0
#define CALIB_H1_LSB 0xE2
#define CALIB_H1_MSB 0xE3
#define CALIB_H2_LSB 0xE2
#define CALIB_H2_MSB 0xE1
#define CALIB_H3_LSB 0xE4
#define CALIB_H4_LSB 0xE5
#define CALIB_H5_LSB 0xE6
#define CALIB_H6_LSB 0xE7
#define CALIB_H7_LSB 0xE8
#define CALIB_G1_LSB 0xED
#define CALIB_G2_LSB 0xEB
#define CALIB_G2_MSB 0xEC
#define CALIB_G3_LSB 0xEE
#define CALIB_RES_HEAT_RANGE 0x02
#define CALIB_RES_HEAT_VAL 0x00

FUNCRESULT bme688Init(BME688Config *config, SPIInstance spi, PinNumber miso, PinNumber mosi, PinNumber sck, PinNumber cs)
{
    config->spi = spi;
    config->cs = cs;
    config->currentMode = BME688_MODE_SLEEP;

    spiInitPins(spi, miso, mosi, sck, cs);

    __bme688SetPage(config, 0);

    BYTE par_t1_lsb = __bme688ReadReg(config, CALIB_T1_LSB);
    BYTE par_t1_msb = __bme688ReadReg(config, CALIB_T1_MSB);
    BYTE par_t2_lsb = __bme688ReadReg(config, CALIB_T2_LSB);
    BYTE par_t2_msb = __bme688ReadReg(config, CALIB_T2_MSB);
    BYTE par_t3_lsb = __bme688ReadReg(config, CALIB_T3_LSB);
    BYTE par_p1_lsb = __bme688ReadReg(config, CALIB_P1_LSB);
    BYTE par_p1_msb = __bme688ReadReg(config, CALIB_P1_MSB);
    BYTE par_p2_lsb = __bme688ReadReg(config, CALIB_P2_LSB);
    BYTE par_p2_msb = __bme688ReadReg(config, CALIB_P2_MSB);
    BYTE par_p3_lsb = __bme688ReadReg(config, CALIB_P3_LSB);
    BYTE par_p4_lsb = __bme688ReadReg(config, CALIB_P4_LSB);
    BYTE par_p4_msb = __bme688ReadReg(config, CALIB_P4_MSB);
    BYTE par_p5_lsb = __bme688ReadReg(config, CALIB_P5_LSB);
    BYTE par_p5_msb = __bme688ReadReg(config, CALIB_P5_MSB);
    BYTE par_p6_lsb = __bme688ReadReg(config, CALIB_P6_LSB);
    BYTE par_p7_lsb = __bme688ReadReg(config, CALIB_P7_LSB);
    BYTE par_p8_lsb = __bme688ReadReg(config, CALIB_P8_LSB);
    BYTE par_p8_msb = __bme688ReadReg(config, CALIB_P8_MSB);
    BYTE par_p9_lsb = __bme688ReadReg(config, CALIB_P9_LSB);
    BYTE par_p9_msb = __bme688ReadReg(config, CALIB_P9_MSB);
    BYTE par_p10_lsb = __bme688ReadReg(config, CALIB_P10_LSB);
    BYTE par_h1_lsb = __bme688ReadReg(config, CALIB_H1_LSB);
    BYTE par_h1_msb = __bme688ReadReg(config, CALIB_H1_MSB);
    BYTE par_h2_lsb = __bme688ReadReg(config, CALIB_H2_LSB);
    BYTE par_h2_msb = __bme688ReadReg(config, CALIB_H2_MSB);
    BYTE par_h3_lsb = __bme688ReadReg(config, CALIB_H3_LSB);
    BYTE par_h4_lsb = __bme688ReadReg(config, CALIB_H4_LSB);
    BYTE par_h5_lsb = __bme688ReadReg(config, CALIB_H5_LSB);
    BYTE par_h6_lsb = __bme688ReadReg(config, CALIB_H6_LSB);
    BYTE par_h7_lsb = __bme688ReadReg(config, CALIB_H7_LSB);
    BYTE par_g1_lsb = __bme688ReadReg(config, CALIB_G1_LSB);
    BYTE par_g2_lsb = __bme688ReadReg(config, CALIB_G2_LSB);
    BYTE par_g2_msb = __bme688ReadReg(config, CALIB_G2_MSB);
    BYTE par_g3_lsb = __bme688ReadReg(config, CALIB_G3_LSB);

    __bme688SetPage(config, 1);

    BYTE par_res_heat_range = __bme688ReadReg(config, CALIB_RES_HEAT_RANGE);
    BYTE par_res_heat_val = __bme688ReadReg(config, CALIB_RES_HEAT_VAL);

    config->par_t1 = (UINT16)((par_t1_msb << 8) | par_t1_lsb);
    config->par_t2 = (INT16)((par_t2_msb << 8) | par_t2_lsb);
    config->par_t3 = (INT8)par_t3_lsb;
    config->par_p1 = (UINT16)((par_p1_msb << 8) | par_p1_lsb);
    config->par_p2 = (INT16)((par_p2_msb << 8) | par_p2_lsb);
    config->par_p3 = (INT8)par_p3_lsb;
    config->par_p4 = (INT16)((par_p4_msb << 8) | par_p4_lsb);
    config->par_p5 = (INT16)((par_p5_msb << 8) | par_p5_lsb);
    config->par_p6 = (INT8)par_p6_lsb;
    config->par_p7 = (INT8)par_p7_lsb;
    config->par_p8 = (INT16)((par_p8_msb << 8) | par_p8_lsb);
    config->par_p9 = (INT16)((par_p9_msb << 8) | par_p9_lsb);
    config->par_p10 = (INT8)par_p10_lsb;
    config->par_h1 = (UINT16)((par_h1_msb << 4) | (par_h1_lsb & 0x0F));
    config->par_h2 = (UINT16)((par_h2_msb << 4) | (par_h2_lsb >> 4));
    config->par_h3 = (INT8)par_h3_lsb;
    config->par_h4 = (INT8)par_h4_lsb;
    config->par_h5 = (INT8)par_h5_lsb;
    config->par_h6 = (UINT8)par_h6_lsb;
    config->par_h7 = (INT8)par_h7_lsb;
    config->par_g1 = (INT8)par_g1_lsb;
    config->par_g2 = (INT16)((par_g2_msb << 8) | par_g2_lsb);
    config->par_g3 = (INT8)par_g3_lsb;
    config->res_heat_range = (UINT8)((par_res_heat_range & 0x30) >> 4);
    config->res_heat_val = (UINT8)par_res_heat_val;

    return SUC_OK;
}

FUNCRESULT bme688Read(BME688Config *config, BME688Data *data)
{
    BYTE press_msb = __bme688ReadReg(config, PRESS_MSB_0);
    BYTE press_lsb = __bme688ReadReg(config, PRESS_LSB_0);
    BYTE press_xlsb = __bme688ReadReg(config, PRESS_XLSB_0);
    BYTE temp_msb = __bme688ReadReg(config, TEMP_MSB_0);
    BYTE temp_lsb = __bme688ReadReg(config, TEMP_LSB_0);
    BYTE temp_xlsb = __bme688ReadReg(config, TEMP_XLSB_0);
    BYTE hum_msb = __bme688ReadReg(config, HUM_MSB_0);
    BYTE hum_lsb = __bme688ReadReg(config, HUM_LSB_0);
    BYTE gas_r_msb = __bme688ReadReg(config, GAS_R_MSB_0);
    BYTE gas_r_lsb = __bme688ReadReg(config, GAS_R_LSB_0);

    UINT32 temp_adc = (UINT32)((temp_msb << 12) | (temp_lsb << 4) | (temp_xlsb >> 4));
    UINT32 press_adc = (UINT32)((press_msb << 12) | (press_lsb << 4) | (press_xlsb >> 4));
    UINT16 hum_adc = (UINT16)((hum_msb << 8) | hum_lsb);
    UINT16 gas_resistance_adc = (UINT16)((gas_r_msb << 2) | (gas_r_lsb >> 6));
    UINT8 gas_range_adc = (UINT8)(gas_r_lsb & 0x0F);

    FLOAT var1, var2, var3, var4, t_fine, temp_comp, press_comp, hum_comp, gas_res;

    var1 = ((((FLOAT)temp_adc / 16384.0f) - ((FLOAT)config->par_t1 / 1024.0f)) * ((FLOAT)config->par_t2));
    var2 = (((((FLOAT)temp_adc / 131072.0f) - ((FLOAT)config->par_t1 / 8192.0f)) * (((FLOAT)temp_adc / 131072.0f) - ((FLOAT)config->par_t1 / 8192.0f))) * ((FLOAT)config->par_t3 * 16.0f));
    t_fine = var1 + var2;
    temp_comp = t_fine / 5120.0f;

    var1 = ((FLOAT)t_fine / 2.0f) - 64000.0f;
    var2 = var1 * var1 * ((FLOAT)config->par_p6 / 131072.0f);
    var2 = var2 + (var1 * (FLOAT)config->par_p5 * 2.0f);
    var2 = (var2 / 4.0f) + ((FLOAT)config->par_p4 * 65536.0f);
    var1 = ((((FLOAT)config->par_t3 * var1 * var1) / 16384.0f) + ((FLOAT)config->par_t2 * var1)) / 524288.0f;
    var1 = (1.0f + (var1 / 32768.0f)) * (FLOAT)config->par_p1;
    press_comp = 1048576.0f - (FLOAT)press_adc;
    press_comp = ((press_comp - (var2 / 4096.0f)) * 6250.0f) / var1;
    var1 = ((FLOAT)config->par_p9 * press_comp * press_comp) / 2147483648.0f;
    var2 = press_comp * ((FLOAT)config->par_p8 / 32768.0f);
    var3 = (press_comp / 256.0f) * (press_comp / 256.0f) * (press_comp / 256.0f) * (config->par_p10 / 131072.0f);
    press_comp = press_comp + (var1 + var2 + var3 + ((FLOAT)config->par_p7 * 128.0f)) / 16.0f;

    var1 = (FLOAT)((FLOAT)hum_adc) - (((FLOAT)config->par_h1 * 16.0f) + (((FLOAT)config->par_h3 / 2.0f) * temp_comp));
    var2 = var1 * ((FLOAT)(((FLOAT)config->par_h2 / 262144.0f) * (1.0f + (((FLOAT)config->par_h4 / 16384.0f) * temp_comp) + (((FLOAT)config->par_h5 / 1048576.0f) * temp_comp * temp_comp))));
    var3 = (FLOAT)config->par_h6 / 16384.0f;
    var4 = (FLOAT)config->par_h7 / 2097152.0f;
    hum_comp = var2 + ((var3 + (var4 * temp_comp)) + var2 * var2);

    // INT32 temp_scaled = (INT32)(temp_comp * 100.0f);
    // INT32 var1h = (INT32)hum_adc - (INT32)((INT32)config->par_h1 << 4) - (((temp_scaled * (INT32)config->par_h3) / ((INT32)100)) >> 1);
    // INT32 var2h = ((INT32)config->par_h2 * (((temp_scaled * (INT32)config->par_h4) / ((INT32)100)) + (((temp_scaled * ((temp_scaled * (INT32)config->par_h5) / ((INT32)100))) >> 6) / ((INT32)100)) + ((INT32)1 << 14))) >> 10;
    // INT32 var3h = var1h * var2h;
    // INT32 var4h = (((INT32)config->par_h6 << 7) + ((temp_scaled * (INT32)config->par_h7) / ((INT32)100))) >> 4;
    // INT32 var5h = ((var3h >> 14) * (var3h >> 14)) >> 10;
    // INT32 var6h = (var4h * var5h) >> 1;
    // INT32 hum_comph = (((var3h + var6h) >> 10) * ((INT32)1000)) >> 12;

    UINT32 var1g = (UINT32)262144 >> gas_range_adc;
    INT32 var2g = (INT32)gas_resistance_adc - (INT32)512;
    var2g *= (INT32)3;
    var2g = (INT32)4096 + var2g;
    gas_res = 1000000.0f * (FLOAT)var1g / (FLOAT)var2g;

    data->temperature = temp_comp;
    data->pressure = press_comp;
    data->humidity = hum_comp;
    data->gas = gas_res;

    return SUC_OK;
}

FUNCRESULT bme688GetVariantId(BME688Config *config, BYTE *variantId)
{
    *variantId = __bme688ReadReg(config, VARIANT_ID);

    return SUC_OK;
}

FUNCRESULT bme688GetChipId(BME688Config *config, BYTE *chipId)
{
    *chipId = __bme688ReadReg(config, CHIP_ID);

    return SUC_OK;
}

FUNCRESULT bme688SetMode(BME688Config *config, BME688Mode mode)
{
    BYTE data = __bme688ReadReg(config, CTRL_MEAS);

    data &= 0xFC;
    data |= mode;

    __bme688WriteReg(config, CTRL_MEAS, data);

    config->currentMode = mode;

    return SUC_OK;
}

FUNCRESULT bme688SetHumidityOSR(BME688Config *config, BME688SensorOSR osr)
{
    BYTE data = __bme688ReadReg(config, CTRL_HUM);

    data &= 0xF8;
    data |= osr;

    __bme688WriteReg(config, CTRL_HUM, data);

    return SUC_OK;
}

FUNCRESULT bme688SetTemperatureOSR(BME688Config *config, BME688SensorOSR osr)
{
    BYTE ctrl_meas = __bme688ReadReg(config, CTRL_MEAS);

    ctrl_meas &= 0x1F;
    ctrl_meas |= osr << 5;

    __bme688WriteReg(config, CTRL_MEAS, ctrl_meas);

    return SUC_OK;
}

FUNCRESULT bme688SetPressureOSR(BME688Config *config, BME688SensorOSR osr)
{
    BYTE ctrl_meas = __bme688ReadReg(config, CTRL_MEAS);

    ctrl_meas &= 0xE3;
    ctrl_meas |= osr << 2;

    __bme688WriteReg(config, CTRL_MEAS, ctrl_meas);

    return SUC_OK;
}

FUNCRESULT bme688SetIIRFilter(BME688Config *config, BME688IIRFilterCoefficient filter)
{
    BYTE cfg = __bme688ReadReg(config, CONFIG);

    cfg &= 0xE3;
    cfg |= filter << 2;

    __bme688WriteReg(config, CONFIG, cfg);

    return SUC_OK;
}

VOID __bme688SetHeaterCurrent(BME688Config *config, UINT8 index, FLOAT current)
{
    __bme688WriteReg(config, IDAC_HEAT_X + index, (BYTE)(current * 8 - 1));
}

VOID __bme688SetTargetHeaterTemp(BME688Config *config, UINT8 index, FLOAT targetTemp)
{
    FLOAT ambTemp = 25.0f;
    FLOAT var1 = ((FLOAT)config->par_g1 / 16.0f) + 49.0;
    FLOAT var2 = (((FLOAT)config->par_g2 / 32768.0f) * 0.0005f) + 0.00235f;
    FLOAT var3 = (FLOAT)config->par_g3 / 1024.0f;
    FLOAT var4 = var1 * (1.0f + (var2 * (FLOAT)targetTemp));
    FLOAT var5 = var4 + (var3 * (FLOAT)ambTemp);
    BYTE res_heat_x = (BYTE)(3.4f * ((var5 * (4.0f / (4.0f + (FLOAT)config->res_heat_range)) * (1.0f / (1.0f + ((FLOAT)config->res_heat_val * 0.002f)))) - 25));

    __bme688WriteReg(config, RES_HEAT_X + index, res_heat_x);
}

VOID __bme688SetParallelSequences(BME688Config *config, UINT8 index, BYTE sequences)
{
    if (config->currentMode == BME688_MODE_PARALLEL)
    {
        __bme688WriteReg(config, GAS_WAIT_X + index, sequences);
    }
}

VOID __bme688SetGasSensorHeaterOnTime(BME688Config *config, UINT8 index, UINT16 time)
{
    UINT8 factor = 0;
    UINT8 durval;

    if (time >= 0xFC0)
    {
        durval = 0xFF;
    }
    else
    {
        while (time > 0x3F)
        {
            time = time / 4;
            factor += 1;
        }

        durval = (UINT8)(time + (factor * 64));
    }

    BYTE address = 0x00;

    if (config->currentMode == BME688_MODE_SLEEP || config->currentMode == BME688_MODE_FORCED)
    {
        address = GAS_WAIT_X + index;
    }
    else if (config->currentMode == BME688_MODE_PARALLEL)
    {
        address = GAS_WAIT_SHARED;
    }

    __bme688WriteReg(config, address, durval);
}

VOID __bme688HeaterOff(BME688Config *config)
{
    BYTE data = __bme688ReadReg(config, CTRL_GAS_0);

    data &= 0xF7;
    data |= 1 << 3;

    __bme688WriteReg(config, CTRL_GAS_0, data);
}

VOID __bme688SetHeaterProfile(BME688Config *config, BYTE index)
{
    BYTE data = __bme688ReadReg(config, CTRL_GAS_1);

    data &= 0xF0;
    data |= index;

    __bme688WriteReg(config, CTRL_GAS_1, data);
}

VOID __bme688RunGas(BME688Config *config)
{
    BYTE data = __bme688ReadReg(config, CTRL_GAS_1);

    data &= 0xDF;
    data |= 1 << 5;

    __bme688WriteReg(config, CTRL_GAS_1, data);
}

BOOL __bme688IsDataReady(BME688Config *config, UINT8 index)
{
    return __bme688ReadReg(config, MEAS_STATUS_0 + index * 0x0B) & 0x80;
}

BOOL __bme688IsMeasuring(BME688Config *config, UINT8 index)
{
    return __bme688ReadReg(config, MEAS_STATUS_0 + index * 0x0B) & 0x40;
}

BOOL __bme688IsConverting(BME688Config *config, UINT8 index)
{
    return __bme688ReadReg(config, MEAS_STATUS_0 + index * 0x0B) & 0x20;
}

BYTE __bme688GetMeasurementIndex(BME688Config *config, UINT8 index)
{
    return __bme688ReadReg(config, MEAS_STATUS_0 + index * 0x0B) & 0x0F;
}

BOOL __bme688IsGasValid(BME688Config *config, UINT8 index)
{
    return __bme688ReadReg(config, GAS_R_LSB_0 + index * 0x0B) & 0x20;
}

BOOL __bme688IsHeaterStable(BME688Config *config, UINT8 index)
{
    return __bme688ReadReg(config, GAS_R_LSB_0 + index * 0x0B) & 0x10;
}

BYTE __bme688GetMeasurementSequenceNumber(BME688Config *config, UINT8 index)
{
    return __bme688ReadReg(config, SUB_MEAS_INDEX_0 + index * 0x0B);
}

VOID __bme688SetPage(BME688Config *config, UINT8 page)
{
    BYTE status = __bme688ReadReg(config, STATUS);

    status &= 0xEF;
    status |= page << 4;

    __bme688WriteReg(config, STATUS, status);
}

VOID __bme688SoftReset(BME688Config *config)
{
    __bme688WriteReg(config, RESET, SOFT_RESET_CMD);
}

BYTE __bme688ReadReg(BME688Config *config, BYTE address)
{
    BYTE data;

    address |= 0x80;

    gpioSetPinState(config->cs, GPIO_LOW);

    spiWriteBlocking(config->spi, &address, 1);
    spiReadBlocking(config->spi, 0, &data, 1);

    gpioSetPinState(config->cs, GPIO_HIGH);

    return data;
}

VOID __bme688ReadRegs(BME688Config *config, BYTE address, BYTE *buffer, SIZE count)
{
    address |= 0x80;

    gpioSetPinState(config->cs, GPIO_LOW);

    spiWriteBlocking(config->spi, &address, 1);
    spiReadBlocking(config->spi, 0, buffer, count);

    gpioSetPinState(config->cs, GPIO_HIGH);
}

VOID __bme688WriteReg(BME688Config *config, BYTE address, BYTE data)
{
    address &= 0x7F;

    gpioSetPinState(config->cs, GPIO_LOW);

    spiWriteBlocking(config->spi, &address, 1);
    spiWriteBlocking(config->spi, &data, 1);

    gpioSetPinState(config->cs, GPIO_HIGH);
}