#pragma once

/**
 * REF: https://github.com/BoschSensortec/BME68x-Sensor-API
 */

#include <obc/api.h>
#include "drivers/gpio/gpio_driver.h"
#include "drivers/gpio/spi_driver.h"

typedef struct BME688Config
{
    SPIInstance spi;
    PinNumber cs;
} BME688Config;

typedef struct BME688Data
{
    FLOAT temperature;
    FLOAT humidity;
    INT32 pressure;
} BME688Data;

typedef enum BME688PowerMode
{
    BME688_POWER_MODE_SLEEP = 0x00,
    BME688_POWER_MODE_FORCED = 0x01,
    BME688_POWER_MODE_PARALLEL = 0x02
} BME688PowerMode;

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

FUNCRESULT bme688Init(BME688Config *config, SPIInstance spi, PinNumber miso, PinNumber mosi, PinNumber sck, PinNumber cs);
FUNCRESULT bme688Read(BME688Config *config, BME688Data *data);

FUNCRESULT bme688GetVariantId(BME688Config *config, BYTE *variantId);
FUNCRESULT bme688GetChipId(BME688Config *config, BYTE *chipId);

FUNCRESULT bme688SetPowerMode(BME688Config *config, BME688PowerMode mode);

FUNCRESULT bme688SetHumidityOSR(BME688Config *config, BME688SensorOSR osr);
FUNCRESULT bme688SetTemperatureOSR(BME688Config *config, BME688SensorOSR osr);
FUNCRESULT bme688SetPressureOSR(BME688Config *config, BME688SensorOSR osr);
FUNCRESULT bme688SetIIRFilter(BME688Config *config, BME688IIRFilterCoefficient filter);

VOID __bme688SetPage(BME688Config *config, BYTE page);
VOID __bme688SoftReset(BME688Config *config);
BYTE __bme688ReadReg(BME688Config *config, BYTE address);
VOID __bme688ReadRegs(BME688Config *config, BYTE address, BYTE *buffer, SIZE count);
VOID __bme688WriteReg(BME688Config *config, BYTE address, BYTE data);