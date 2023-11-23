#include "modules/drivers/barometer/bme688_driver.h"
#include "modules/drivers/hal/time_tracker.h"

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

#define BME688_VARIANT_ID 0x01
#define BME688_CHIP_ID 0x61
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

static void _bme688_set_page(bme688_config_t *config, uint8_t page);
static void _bme688_soft_reset(bme688_config_t *config);
static bool _bme688_write_reg_field(bme688_config_t *config, uint8_t address, uint8_t length, uint8_t offset, uint8_t value);
static uint8_t _bme688_read_reg(bme688_config_t *config, uint8_t address);
static void _bme688_read_regs(bme688_config_t *config, uint8_t address, uint8_t *buffer, size_t count);
static void _bme688_write_reg(bme688_config_t *config, uint8_t address, uint8_t data);

void bme688_init(bme688_config_t *config, hal_spi_instance_t spi, hal_pin_number_t miso, hal_pin_number_t mosi, hal_pin_number_t sck, hal_pin_number_t cs)
{
    config->spi = spi;
    config->cs = cs;
    config->currentMode = BME688_MODE_SLEEP;

    hal_spi_init_pins(spi, miso, mosi, sck, cs);

    _bme688_set_page(config, 0);

    uint8_t par_t1_lsb = _bme688_read_reg(config, CALIB_T1_LSB);
    uint8_t par_t1_msb = _bme688_read_reg(config, CALIB_T1_MSB);
    uint8_t par_t2_lsb = _bme688_read_reg(config, CALIB_T2_LSB);
    uint8_t par_t2_msb = _bme688_read_reg(config, CALIB_T2_MSB);
    uint8_t par_t3_lsb = _bme688_read_reg(config, CALIB_T3_LSB);
    uint8_t par_p1_lsb = _bme688_read_reg(config, CALIB_P1_LSB);
    uint8_t par_p1_msb = _bme688_read_reg(config, CALIB_P1_MSB);
    uint8_t par_p2_lsb = _bme688_read_reg(config, CALIB_P2_LSB);
    uint8_t par_p2_msb = _bme688_read_reg(config, CALIB_P2_MSB);
    uint8_t par_p3_lsb = _bme688_read_reg(config, CALIB_P3_LSB);
    uint8_t par_p4_lsb = _bme688_read_reg(config, CALIB_P4_LSB);
    uint8_t par_p4_msb = _bme688_read_reg(config, CALIB_P4_MSB);
    uint8_t par_p5_lsb = _bme688_read_reg(config, CALIB_P5_LSB);
    uint8_t par_p5_msb = _bme688_read_reg(config, CALIB_P5_MSB);
    uint8_t par_p6_lsb = _bme688_read_reg(config, CALIB_P6_LSB);
    uint8_t par_p7_lsb = _bme688_read_reg(config, CALIB_P7_LSB);
    uint8_t par_p8_lsb = _bme688_read_reg(config, CALIB_P8_LSB);
    uint8_t par_p8_msb = _bme688_read_reg(config, CALIB_P8_MSB);
    uint8_t par_p9_lsb = _bme688_read_reg(config, CALIB_P9_LSB);
    uint8_t par_p9_msb = _bme688_read_reg(config, CALIB_P9_MSB);
    uint8_t par_p10_lsb = _bme688_read_reg(config, CALIB_P10_LSB);
    uint8_t par_h1_lsb = _bme688_read_reg(config, CALIB_H1_LSB);
    uint8_t par_h1_msb = _bme688_read_reg(config, CALIB_H1_MSB);
    uint8_t par_h2_lsb = _bme688_read_reg(config, CALIB_H2_LSB);
    uint8_t par_h2_msb = _bme688_read_reg(config, CALIB_H2_MSB);
    uint8_t par_h3_lsb = _bme688_read_reg(config, CALIB_H3_LSB);
    uint8_t par_h4_lsb = _bme688_read_reg(config, CALIB_H4_LSB);
    uint8_t par_h5_lsb = _bme688_read_reg(config, CALIB_H5_LSB);
    uint8_t par_h6_lsb = _bme688_read_reg(config, CALIB_H6_LSB);
    uint8_t par_h7_lsb = _bme688_read_reg(config, CALIB_H7_LSB);
    uint8_t par_g1_lsb = _bme688_read_reg(config, CALIB_G1_LSB);
    uint8_t par_g2_lsb = _bme688_read_reg(config, CALIB_G2_LSB);
    uint8_t par_g2_msb = _bme688_read_reg(config, CALIB_G2_MSB);
    uint8_t par_g3_lsb = _bme688_read_reg(config, CALIB_G3_LSB);

    _bme688_set_page(config, 1);

    uint8_t par_res_heat_range = _bme688_read_reg(config, CALIB_RES_HEAT_RANGE);
    uint8_t par_res_heat_val = _bme688_read_reg(config, CALIB_RES_HEAT_VAL);

    config->par_t1 = (uint16_t)((par_t1_msb << 8) | par_t1_lsb);
    config->par_t2 = (int16_t)((par_t2_msb << 8) | par_t2_lsb);
    config->par_t3 = (int8_t)par_t3_lsb;
    config->par_p1 = (uint16_t)((par_p1_msb << 8) | par_p1_lsb);
    config->par_p2 = (int16_t)((par_p2_msb << 8) | par_p2_lsb);
    config->par_p3 = (int8_t)par_p3_lsb;
    config->par_p4 = (int16_t)((par_p4_msb << 8) | par_p4_lsb);
    config->par_p5 = (int16_t)((par_p5_msb << 8) | par_p5_lsb);
    config->par_p6 = (int8_t)par_p6_lsb;
    config->par_p7 = (int8_t)par_p7_lsb;
    config->par_p8 = (int16_t)((par_p8_msb << 8) | par_p8_lsb);
    config->par_p9 = (int16_t)((par_p9_msb << 8) | par_p9_lsb);
    config->par_p10 = (int8_t)par_p10_lsb;
    config->par_h1 = (uint16_t)((par_h1_msb << 4) | (par_h1_lsb & 0x0F));
    config->par_h2 = (uint16_t)((par_h2_msb << 4) | (par_h2_lsb >> 4));
    config->par_h3 = (int8_t)par_h3_lsb;
    config->par_h4 = (int8_t)par_h4_lsb;
    config->par_h5 = (int8_t)par_h5_lsb;
    config->par_h6 = (uint8_t)par_h6_lsb;
    config->par_h7 = (int8_t)par_h7_lsb;
    config->par_g1 = (int8_t)par_g1_lsb;
    config->par_g2 = (int16_t)((par_g2_msb << 8) | par_g2_lsb);
    config->par_g3 = (int8_t)par_g3_lsb;
    config->res_heat_range = (uint8_t)((par_res_heat_range & 0x30) >> 4);
    config->res_heat_val = (uint8_t)par_res_heat_val;
}

static float _calculateTemperature(bme688_config_t *config, unsigned int temp_adc, int *t_fine)
{
    float var1 = ((((float)temp_adc / 16384.0f) - ((float)config->par_t1 / 1024.0f)) * ((float)config->par_t2));
    float var2 = (((((float)temp_adc / 131072.0f) - ((float)config->par_t1 / 8192.0f)) * (((float)temp_adc / 131072.0f) - ((float)config->par_t1 / 8192.0f))) * ((float)config->par_t3 * 16.0f));
    float t_fine_f = var1 + var2;

    int var1t = ((int)temp_adc >> 3) - ((int)config->par_t1 << 1);
    int var2t = (var1t * (int)config->par_t2) >> 11;
    int var3t = ((((var1t >> 1) * (var1t >> 1)) >> 12) * ((int)config->par_t3 << 4)) >> 14;

    *t_fine = (float)(var2t + var3t);

    return t_fine_f / 5120.0f;
}

static int _calculatePressure(bme688_config_t *config, unsigned int press_adc, int t_fine)
{
    int var1p = (int)((int)t_fine >> 1) - 64000;
    int var2p = ((((var1p >> 2) * (var1p >> 2)) >> 11) * (int)config->par_p6) >> 2;
    var2p = var2p + ((var1p * (int)config->par_p5) << 1);
    var2p = (var2p >> 2) + ((int)config->par_p4 << 16);
    var1p = (((((var1p >> 2) * (var1p >> 2)) >> 13) * ((int)config->par_p3 << 5)) >> 3) + (((int)config->par_p2 * var1p) >> 1);
    var1p = var1p >> 18;
    var1p = ((32768 + var1p) * (int)config->par_p1) >> 15;
    int press_comp_p = 1048576 - press_adc;
    press_comp_p = (int)((press_comp_p - (var2p >> 12)) * (unsigned int)3125);

    if (press_comp_p >= (1 << 30))
    {
        press_comp_p = ((press_comp_p / (unsigned int)var1p) << 1);
    }
    else
    {
        press_comp_p = ((press_comp_p << 1) / (unsigned int)var1p);
    }

    var1p = ((int)config->par_p9 * (((press_comp_p >> 3) * (press_comp_p >> 3)) >> 13)) >> 12;
    var2p = (((int)(press_comp_p >> 2)) * ((int)config->par_p8)) >> 13;
    int var3p = (((int)(press_comp_p >> 8)) * ((int)(press_comp_p >> 8)) * ((int)(press_comp_p >> 8)) * ((int)config->par_p10)) >> 17;
    press_comp_p = (int)(press_comp_p) + ((var1p + var2p + var3p + ((int)((int)config->par_p7 << 7))) >> 4);

    return press_comp_p;
}

static float _calculateHumidity(bme688_config_t *config, uint16_t hum_adc, float temp_comp)
{
    int temp_scaled = (int)(temp_comp * 100.0f);
    int var1h = (int)hum_adc - (int)((int)config->par_h1 << 4) - (((temp_scaled * (int)config->par_h3) / ((int)100)) >> 1);
    int var2h = ((int)config->par_h2 * (((temp_scaled * (int)config->par_h4) / ((int)100)) + (((temp_scaled * ((temp_scaled * (int)config->par_h5) / ((int)100))) >> 6) / ((int)100)) + ((int)1 << 14))) >> 10;
    int var3h = var1h * var2h;
    int var4h = (((int)config->par_h6 << 7) + ((temp_scaled * (int)config->par_h7) / ((int)100))) >> 4;
    int var5h = ((var3h >> 14) * (var3h >> 14)) >> 10;
    int var6h = (var4h * var5h) >> 1;
    int hum_comph = (((var3h + var6h) >> 10) * ((int)1000)) >> 12;

    return (float)((float)hum_comph / 1000.0f);
}

static int _calculateGasRes(bme688_config_t *config, uint16_t gas_resistance_adc, uint8_t gas_range_adc)
{
    unsigned int var1g = (unsigned int)262144 >> gas_range_adc;
    int var2g = (int)gas_resistance_adc - (int)512;

    var2g *= (int)3;
    var2g = (int)4096 + var2g;

    return (int)(1000000.0f * (float)var1g / (float)var2g);
}

void bme688_read(bme688_config_t *config, bme688_data_t *data)
{
    uint8_t press_msb = _bme688_read_reg(config, PRESS_MSB_0);
    uint8_t press_lsb = _bme688_read_reg(config, PRESS_LSB_0);
    uint8_t press_xlsb = _bme688_read_reg(config, PRESS_XLSB_0);
    uint8_t temp_msb = _bme688_read_reg(config, TEMP_MSB_0);
    uint8_t temp_lsb = _bme688_read_reg(config, TEMP_LSB_0);
    uint8_t temp_xlsb = _bme688_read_reg(config, TEMP_XLSB_0);
    uint8_t hum_msb = _bme688_read_reg(config, HUM_MSB_0);
    uint8_t hum_lsb = _bme688_read_reg(config, HUM_LSB_0);
    uint8_t gas_r_msb = _bme688_read_reg(config, GAS_R_MSB_0);
    uint8_t gas_r_lsb = _bme688_read_reg(config, GAS_R_LSB_0);

    unsigned int temp_adc = (unsigned int)((temp_msb << 12) | (temp_lsb << 4) | (temp_xlsb >> 4));
    unsigned int press_adc = (unsigned int)((press_msb << 12) | (press_lsb << 4) | (press_xlsb >> 4));
    uint16_t hum_adc = (uint16_t)((hum_msb << 8) | hum_lsb);
    uint16_t gas_resistance_adc = (uint16_t)((gas_r_msb << 2) | (gas_r_lsb >> 6));
    uint8_t gas_range_adc = (uint8_t)(gas_r_lsb & 0x0F);

    int t_fine = 0;

    data->temperature = _calculateTemperature(config, temp_adc, &t_fine);
    data->pressure = _calculatePressure(config, press_adc, (int)t_fine);
    data->humidity = _calculateHumidity(config, hum_adc, data->temperature);
    data->gas = _calculateGasRes(config, gas_resistance_adc, gas_range_adc);
}

void bme688_validate_id(bme688_config_t *config, bool *valid)
{
    uint8_t variantId = _bme688_read_reg(config, VARIANT_ID);
    uint8_t chipId = _bme688_read_reg(config, CHIP_ID);

    *valid = (variantId == BME688_VARIANT_ID) && (chipId == BME688_CHIP_ID);
}

void bme688_set_mode(bme688_config_t *config, bme688_mode_t mode)
{
    _bme688_write_reg_field(config, CTRL_MEAS, 2, 0, mode);

    config->currentMode = mode;
}

void bme688_set_config(bme688_config_t *config, bme688_sensor_osr_t temperatureOSR, bme688_sensor_osr_t pressureOSR, bme688_sensor_osr_t humidityOSR, bme688_i_i_r_filter_coefficient_t filterCoefficient)
{
    _bme688_write_reg_field(config, CTRL_HUM, 3, 0, humidityOSR);
    _bme688_write_reg_field(config, CTRL_MEAS, 6, 2, temperatureOSR << 3 | pressureOSR);
    _bme688_write_reg_field(config, CONFIG, 3, 2, filterCoefficient);
}

void bme688_set_heater_config(bme688_config_t *config, uint8_t index, float current, float targetTemp, uint8_t sequences, uint16_t time)
{
    if (index > 9)
    {
        return;
    }

    _bme688_write_reg_field(config, CTRL_GAS_1, 1, 5, 1);
    _bme688_write_reg_field(config, CTRL_GAS_1, 3, 0, index);

    if (current > 0)
    {
        _bme688_write_reg(config, IDAC_HEAT_X + index, (uint8_t)(current * 8 - 1));
    }

    float ambTemp = 25.0f;
    float var1 = ((float)config->par_g1 / 16.0f) + 49.0;
    float var2 = (((float)config->par_g2 / 32768.0f) * 0.0005f) + 0.00235f;
    float var3 = (float)config->par_g3 / 1024.0f;
    float var4 = var1 * (1.0f + (var2 * (float)targetTemp));
    float var5 = var4 + (var3 * (float)ambTemp);
    uint8_t res_heat_x = (uint8_t)(3.4f * ((var5 * (4.0f / (4.0f + (float)config->res_heat_range)) * (1.0f / (1.0f + ((float)config->res_heat_val * 0.002f)))) - 25));

    _bme688_write_reg(config, RES_HEAT_X + index, res_heat_x);

    uint8_t factor = 0;
    uint8_t durval = 0;

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

        durval = (uint8_t)(time + (factor * 64));
    }

    uint8_t address = 0x00;

    if (config->currentMode == BME688_MODE_SLEEP || config->currentMode == BME688_MODE_FORCED)
    {
        address = GAS_WAIT_X + index;
    }
    else if (config->currentMode == BME688_MODE_PARALLEL)
    {
        _bme688_write_reg(config, GAS_WAIT_X + index, sequences);

        address = GAS_WAIT_SHARED;
    }

    _bme688_write_reg(config, address, durval);
}

void bme688_heater_off(bme688_config_t *config)
{
    _bme688_write_reg_field(config, CTRL_GAS_0, 1, 3, 1);
}

bool bme688_is_data_ready(bme688_config_t *config, uint8_t index)
{
    return _bme688_read_reg(config, MEAS_STATUS_0 + index * 0x0B) & 0x80;
}

bool bme688_is_measuring(bme688_config_t *config, uint8_t index)
{
    return _bme688_read_reg(config, MEAS_STATUS_0 + index * 0x0B) & 0x40;
}

bool bme688_is_converting(bme688_config_t *config, uint8_t index)
{
    return _bme688_read_reg(config, MEAS_STATUS_0 + index * 0x0B) & 0x20;
}

uint8_t bme688_get_measurement_index(bme688_config_t *config, uint8_t index)
{
    return _bme688_read_reg(config, MEAS_STATUS_0 + index * 0x0B) & 0x0F;
}

bool bme688_is_gas_valid(bme688_config_t *config, uint8_t index)
{
    return _bme688_read_reg(config, GAS_R_LSB_0 + index * 0x0B) & 0x20;
}

bool bme688_is_heater_stable(bme688_config_t *config, uint8_t index)
{
    return _bme688_read_reg(config, GAS_R_LSB_0 + index * 0x0B) & 0x10;
}

uint8_t bme688_get_measurement_sequence_number(bme688_config_t *config, uint8_t index)
{
    return _bme688_read_reg(config, SUB_MEAS_INDEX_0 + index * 0x0B);
}

static void _bme688_set_page(bme688_config_t *config, uint8_t page)
{
    _bme688_write_reg_field(config, STATUS, 1, 4, page);
}

static void _bme688_soft_reset(bme688_config_t *config)
{
    _bme688_write_reg(config, RESET, SOFT_RESET_CMD);

    hal_time_sleep_ms(10);
}

static bool _bme688_write_reg_field(bme688_config_t *config, uint8_t address, uint8_t length, uint8_t offset, uint8_t value)
{
    uint8_t data = _bme688_read_reg(config, address);

    uint8_t mask = 0xFF;
    mask >>= offset;
    mask <<= offset;
    mask <<= 8 - offset - length;
    mask >>= 8 - offset - length;

    data &= ~mask;
    data |= (value << offset);

    _bme688_write_reg(config, address, data);

#if !NDEBUG
    uint8_t read = _bme688_read_reg(config, address);

    if (read != data)
    {
        return false;
    }
#endif

    return true;
}

static uint8_t _bme688_read_reg(bme688_config_t *config, uint8_t address)
{
    uint8_t data;

    address |= 0x80;

    hal_spi_cs_select(config->cs);

    hal_spi_write(config->spi, &address, 1);
    hal_spi_read(config->spi, 0, &data, 1);

    hal_spi_cs_deselect(config->cs);

    return data;
}

static void _bme688_read_regs(bme688_config_t *config, uint8_t address, uint8_t *buffer, size_t count)
{
    address |= 0x80;

    hal_spi_cs_select(config->cs);

    hal_spi_write(config->spi, &address, 1);
    hal_spi_read(config->spi, 0, buffer, count);

    hal_spi_cs_deselect(config->cs);
}

static void _bme688_write_reg(bme688_config_t *config, uint8_t address, uint8_t data)
{
    address &= 0x7F;

    hal_spi_cs_select(config->cs);

    hal_spi_write(config->spi, &address, 1);
    hal_spi_write(config->spi, &data, 1);

    hal_spi_cs_deselect(config->cs);
}