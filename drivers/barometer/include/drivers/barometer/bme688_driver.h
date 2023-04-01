#pragma once

/**
 * REF: https://github.com/BoschSensortec/BME68x-Sensor-API
 */

#include <obc/api.h>
#include "drivers/gpio/gpio_driver.h"
#include "drivers/gpio/spi_driver.h"

typedef enum BME688Mode
{
    BME688_MODE_SLEEP = 0x00,
    BME688_MODE_FORCED = 0x01,
    BME688_MODE_PARALLEL = 0x02
} BME688Mode;

typedef enum BME688SensorOSR
{
    BME688_SENSOR_OSR_SKIP = 0x00,
    BME688_SENSOR_OSR_1X = 0x01,
    BME688_SENSOR_OSR_2X = 0x02,
    BME688_SENSOR_OSR_4X = 0x03,
    BME688_SENSOR_OSR_8X = 0x04,
    BME688_SENSOR_OSR_16X = 0x05
} BME688SensorOSR;

typedef enum BME688IIRFilterCoefficient
{
    BME688_IIR_FILTER_COEFF_OFF = 0x00,
    BME688_IIR_FILTER_COEFF_1 = 0x01,
    BME688_IIR_FILTER_COEFF_3 = 0x02,
    BME688_IIR_FILTER_COEFF_7 = 0x03,
    BME688_IIR_FILTER_COEFF_15 = 0x04,
    BME688_IIR_FILTER_COEFF_31 = 0x05,
    BME688_IIR_FILTER_COEFF_63 = 0x06,
    BME688_IIR_FILTER_COEFF_127 = 0x07
} BME688IIRFilterCoefficient;

typedef struct BME688Config
{
    SPIInstance spi;
    PinNumber cs;
    BME688Mode currentMode;
    UINT16 par_t1;
    UINT16 par_t2;
    UINT8 par_t3;
    UINT16 par_p1;
    UINT16 par_p2;
    UINT8 par_p3;
    UINT16 par_p4;
    UINT16 par_p5;
    UINT8 par_p6;
    UINT8 par_p7;
    UINT16 par_p8;
    UINT16 par_p9;
    UINT8 par_p10;
    UINT16 par_h1;
    UINT16 par_h2;
    UINT8 par_h3;
    UINT8 par_h4;
    UINT8 par_h5;
    UINT8 par_h6;
    UINT8 par_h7;
    UINT8 par_g1;
    UINT16 par_g2;
    UINT8 par_g3;
    UINT8 res_heat_range;
    UINT8 res_heat_val;
} BME688Config;

typedef struct BME688Data
{
    FLOAT temperature;
    FLOAT humidity;
    FLOAT pressure;
    FLOAT gas;
} BME688Data;

FUNCRESULT bme688Init(BME688Config *config, SPIInstance spi, PinNumber miso, PinNumber mosi, PinNumber sck, PinNumber cs);
FUNCRESULT bme688Read(BME688Config *config, BME688Data *data);

FUNCRESULT bme688GetVariantId(BME688Config *config, BYTE *variantId);
FUNCRESULT bme688GetChipId(BME688Config *config, BYTE *chipId);

FUNCRESULT bme688SetMode(BME688Config *config, BME688Mode mode);

FUNCRESULT bme688SetHumidityOSR(BME688Config *config, BME688SensorOSR osr);
FUNCRESULT bme688SetTemperatureOSR(BME688Config *config, BME688SensorOSR osr);
FUNCRESULT bme688SetPressureOSR(BME688Config *config, BME688SensorOSR osr);
FUNCRESULT bme688SetIIRFilter(BME688Config *config, BME688IIRFilterCoefficient filter);

VOID __bme688SetHeaterCurrent(BME688Config *config, BYTE index, FLOAT current);
VOID __bme688SetTargetHeaterTemp(BME688Config *config, BYTE index, FLOAT targetTemp);
VOID __bme688SetParallelSequences(BME688Config *config, BYTE index, BYTE sequences);
VOID __bme688SetGasSensorHeaterOnTime(BME688Config *config, BYTE index, UINT16 time);
VOID __bme688HeaterOff(BME688Config *config);
VOID __bme688SetHeaterProfile(BME688Config *config, BYTE index);
VOID __bme688RunGas(BME688Config *config);

BOOL __bme688IsDataReady(BME688Config *config, BYTE index);
BOOL __bme688IsMeasuring(BME688Config *config, BYTE index);
BOOL __bme688IsConverting(BME688Config *config, BYTE index);
BYTE __bme688GetMeasurementIndex(BME688Config *config, BYTE index);
BOOL __bme688IsGasValid(BME688Config *config, BYTE index);
BOOL __bme688IsHeaterStable(BME688Config *config, BYTE index);
BYTE __bme688GetMeasurementSequenceNumber(BME688Config *config, BYTE index);

VOID __bme688SetPage(BME688Config *config, BYTE page);
VOID __bme688SoftReset(BME688Config *config);

BYTE __bme688ReadReg(BME688Config *config, BYTE address);
VOID __bme688ReadRegs(BME688Config *config, BYTE address, BYTE *buffer, SIZE count);
VOID __bme688WriteReg(BME688Config *config, BYTE address, BYTE data);