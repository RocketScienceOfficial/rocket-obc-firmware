#pragma once

/**
 * REF: https://github.com/BoschSensortec/BME68x-Sensor-API
 */

#include <obc/api.h>
#include "drivers/gpio/gpio_driver.h"
#include "drivers/gpio/spi_driver.h"

/**
 * @brief BME688 Mode
 */
typedef enum BME688Mode
{
    BME688_MODE_SLEEP = 0x00,
    BME688_MODE_FORCED = 0x01,
    BME688_MODE_PARALLEL = 0x02
} BME688Mode;

/**
 * @brief BME688 Oversampling Rate
 */
typedef enum BME688SensorOSR
{
    BME688_SENSOR_OSR_SKIP = 0x00,
    BME688_SENSOR_OSR_1X = 0x01,
    BME688_SENSOR_OSR_2X = 0x02,
    BME688_SENSOR_OSR_4X = 0x03,
    BME688_SENSOR_OSR_8X = 0x04,
    BME688_SENSOR_OSR_16X = 0x05
} BME688SensorOSR;

/**
 * @brief BME688 IIR Filter Coefficient
 */
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

/**
 * @brief BME688 Configuration
 */
typedef struct BME688Config
{
    SPIInstance spi;
    PinNumber cs;
    BME688Mode currentMode;
    UINT16 par_t1;
    INT16 par_t2;
    INT8 par_t3;
    UINT16 par_p1;
    INT16 par_p2;
    INT8 par_p3;
    INT16 par_p4;
    INT16 par_p5;
    INT8 par_p6;
    INT8 par_p7;
    INT16 par_p8;
    INT16 par_p9;
    INT8 par_p10;
    UINT16 par_h1;
    UINT16 par_h2;
    INT8 par_h3;
    INT8 par_h4;
    INT8 par_h5;
    UINT8 par_h6;
    INT8 par_h7;
    INT8 par_g1;
    INT16 par_g2;
    UINT8 par_g3;
    UINT8 res_heat_range;
    INT8 res_heat_val;
} BME688Config;

/**
 * @brief BME688 Data
 */
typedef struct BME688Data
{
    FLOAT temperature; /** Temperature in Celsius */
    FLOAT humidity;    /** Humidity in percentage */
    INT32 pressure;    /** Pressure in Pascals */
    INT32 gas;         /** Gas in Ohms */
} BME688Data;

/**
 * @brief Initialize BME688 sensor
 *
 * @param config BME688 configuration
 * @param spi SPI instance
 * @param miso MISO pin
 * @param mosi MOSI pin
 * @param sck SCK pin
 * @param cs CS pin
 * @return Result code
 */
FUNCRESULT bme688Init(BME688Config *config, SPIInstance spi, PinNumber miso, PinNumber mosi, PinNumber sck, PinNumber cs);

/**
 * @brief Read BME688 sensor data
 *
 * @param config BME688 configuration
 * @param data BME688 data pointer
 * @return Result code
 */
FUNCRESULT bme688Read(BME688Config *config, BME688Data *data);

/**
 * @brief Validate BME688 sensor ID
 *
 * @param config BME688 configuration
 * @param valid Validity pointer
 * @return Result code
 */
FUNCRESULT bme688ValidateId(BME688Config *config, BOOL *valid);

/**
 * @brief Set BME688 sensor mode
 *
 * @param config BME688 configuration
 * @param mode BME688 mode
 * @return Result code
 */
FUNCRESULT bme688SetMode(BME688Config *config, BME688Mode mode);

/**
 * @brief Set BME688 sensor configuration
 *
 * @param config BME688 configuration
 * @param temperatureOSR Temperature oversampling rate
 * @param pressureOSR Pressure oversampling rate
 * @param humidityOSR Humidity oversampling rate
 * @param filterCoefficient Filter coefficient
 * @return Result code
 */
FUNCRESULT bme688SetConfig(BME688Config *config, BME688SensorOSR temperatureOSR, BME688SensorOSR pressureOSR, BME688SensorOSR humidityOSR, BME688IIRFilterCoefficient filterCoefficient);

/**
 * @brief Set BME688 sensor gas configuration
 *
 * @param config BME688 configuration
 * @param index Heater profile index
 * @param current Heater current in milliamps
 * @param targetTemp Heater target temperature in Celsius
 * @param sequences Heater sequences
 * @param time Heater time in milliseconds
 */
FUNCRESULT bme688SetHeaterConfig(BME688Config *config, UINT8 index, FLOAT current, FLOAT targetTemp, BYTE sequences, UINT16 time);

/**
 * @brief Disable BME688 heater
 *
 * @param config BME688 configuration
 */
FUNCRESULT bme688HeaterOff(BME688Config *config);

/**
 * @brief Check if data is ready for reading
 *
 * @param config BME688 configuration
 * @param index Heater profile index
 * @return TRUE if data is ready, FALSE otherwise
 */
BOOL bme688IsDataReady(BME688Config *config, UINT8 index);

/**
 * @brief Check if data is ready for reading
 *
 * @param config BME688 configuration
 * @param index Heater profile index
 * @return TRUE if data is ready, FALSE otherwise
 */
BOOL bme688IsMeasuring(BME688Config *config, UINT8 index);

/**
 * @brief Check if data is ready for reading
 *
 * @param config BME688 configuration
 * @param index Heater profile index
 * @return TRUE if data is ready, FALSE otherwise
 */
BOOL bme688IsConverting(BME688Config *config, UINT8 index);

/**
 * @brief Check if data is ready for reading
 *
 * @param config BME688 configuration
 * @param index Heater profile index
 * @return TRUE if data is ready, FALSE otherwise
 */
BYTE bme688GetMeasurementIndex(BME688Config *config, UINT8 index);

/**
 * @brief Check if data is ready for reading
 *
 * @param config BME688 configuration
 * @param index Heater profile index
 * @return TRUE if data is ready, FALSE otherwise
 */
BOOL bme688IsGasValid(BME688Config *config, UINT8 index);

/**
 * @brief Check if data is ready for reading
 *
 * @param config BME688 configuration
 * @param index Heater profile index
 * @return TRUE if data is ready, FALSE otherwise
 */
BOOL bme688IsHeaterStable(BME688Config *config, UINT8 index);

/**
 * @brief Check if data is ready for reading
 *
 * @param config BME688 configuration
 * @param index Heater profile index
 * @return TRUE if data is ready, FALSE otherwise
 */
BYTE bme688GetMeasurementSequenceNumber(BME688Config *config, UINT8 index);

/**
 * @brief Set page of BME688 sensor
 *
 * @param config BME688 configuration
 * @param page Page (0 or 1)
 */
VOID __bme688SetPage(BME688Config *config, BYTE page);

/**
 * @brief Reset BME688 sensor
 *
 * @param config BME688 configuration
 */
VOID __bme688SoftReset(BME688Config *config);

/**
 * @brief Write BME688 sensor register field
 *
 * @param config BME688 configuration
 * @param address Register address
 * @param length Field length
 * @param offset Field offset
 * @param value Field value
 * @return TRUE if success, FALSE otherwise
 */
BOOL __bme688WriteRegField(BME688Config *config, BYTE address, UINT8 length, UINT8 offset, BYTE value);

/**
 * @brief Read BME688 sensor register
 *
 * @param config BME688 configuration
 * @param address Register address
 * @return Register value
 */
BYTE __bme688ReadReg(BME688Config *config, BYTE address);

/**
 * @brief Read BME688 sensor registers
 *
 * @param config BME688 configuration
 * @param address Register address
 * @param buffer Buffer pointer
 * @param count Number of bytes to read
 */
VOID __bme688ReadRegs(BME688Config *config, BYTE address, BYTE *buffer, SIZE count);

/**
 * @brief Write BME688 sensor register
 *
 * @param config BME688 configuration
 * @param address Register address
 * @param data Register data
 */
VOID __bme688WriteReg(BME688Config *config, BYTE address, BYTE data);