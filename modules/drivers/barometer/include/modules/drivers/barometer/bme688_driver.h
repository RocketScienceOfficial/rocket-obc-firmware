#ifndef _BME688_DRIVER_H
#define _BME688_DRIVER_H

/**
 * REF: https://github.com/BoschSensortec/BME68x-Sensor-API
 */

#include "modules/drivers/hal/gpio_driver.h"
#include "modules/drivers/hal/spi_driver.h"
#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

/**
 * @brief BME688 Mode
 */
typedef enum bme688_mode
{
    BME688_MODE_SLEEP = 0x00,
    BME688_MODE_FORCED = 0x01,
    BME688_MODE_PARALLEL = 0x02
} bme688_mode_t;

/**
 * @brief BME688 Oversampling Rate
 */
typedef enum bme688_sensor_osr
{
    BME688_SENSOR_OSR_SKIP = 0x00,
    BME688_SENSOR_OSR_1X = 0x01,
    BME688_SENSOR_OSR_2X = 0x02,
    BME688_SENSOR_OSR_4X = 0x03,
    BME688_SENSOR_OSR_8X = 0x04,
    BME688_SENSOR_OSR_16X = 0x05
} bme688_sensor_osr_t;

/**
 * @brief BME688 IIR Filter Coefficient
 */
typedef enum bme688_i_i_r_filter_coefficient
{
    BME688_IIR_FILTER_COEFF_OFF = 0x00,
    BME688_IIR_FILTER_COEFF_1 = 0x01,
    BME688_IIR_FILTER_COEFF_3 = 0x02,
    BME688_IIR_FILTER_COEFF_7 = 0x03,
    BME688_IIR_FILTER_COEFF_15 = 0x04,
    BME688_IIR_FILTER_COEFF_31 = 0x05,
    BME688_IIR_FILTER_COEFF_63 = 0x06,
    BME688_IIR_FILTER_COEFF_127 = 0x07
} bme688_i_i_r_filter_coefficient_t;

/**
 * @brief BME688 Configuration
 */
typedef struct bme688_config
{
    hal_spi_instance_t spi;
    hal_pin_number_t cs;
    bme688_mode_t currentMode;
    uint16_t par_t1;
    int16_t par_t2;
    int8_t par_t3;
    uint16_t par_p1;
    int16_t par_p2;
    int8_t par_p3;
    int16_t par_p4;
    int16_t par_p5;
    int8_t par_p6;
    int8_t par_p7;
    int16_t par_p8;
    int16_t par_p9;
    int8_t par_p10;
    uint16_t par_h1;
    uint16_t par_h2;
    int8_t par_h3;
    int8_t par_h4;
    int8_t par_h5;
    uint8_t par_h6;
    int8_t par_h7;
    int8_t par_g1;
    int16_t par_g2;
    uint8_t par_g3;
    uint8_t res_heat_range;
    int8_t res_heat_val;
} bme688_config_t;

/**
 * @brief BME688 Data
 */
typedef struct bme688_data
{
    float temperature; /** Temperature in Celsius */
    float humidity;    /** Humidity in percentage */
    int pressure;      /** Pressure in Pascals */
    int gas;           /** Gas in Ohms */
} bme688_data_t;

/**
 * @brief Initialize BME688 sensor
 *
 * @param config BME688 configuration
 * @param spi SPI instance
 * @param cs CS pin
 */
void bme688_init(bme688_config_t *config, hal_spi_instance_t spi, hal_pin_number_t cs);

/**
 * @brief Read BME688 sensor data
 *
 * @param config BME688 configuration
 * @param data BME688 data pointer
 */
void bme688_read(bme688_config_t *config, bme688_data_t *data);

/**
 * @brief Validate BME688 sensor ID
 *
 * @param config BME688 configuration
 * @param valid Validity pointer
 */
void bme688_validate_id(bme688_config_t *config, bool *valid);

/**
 * @brief Set BME688 sensor mode
 *
 * @param config BME688 configuration
 * @param mode BME688 mode
 */
void bme688_set_mode(bme688_config_t *config, bme688_mode_t mode);

/**
 * @brief Set BME688 sensor configuration
 *
 * @param config BME688 configuration
 * @param temperatureOSR Temperature oversampling rate
 * @param pressureOSR Pressure oversampling rate
 * @param humidityOSR Humidity oversampling rate
 * @param filterCoefficient Filter coefficient
 */
void bme688_set_config(bme688_config_t *config, bme688_sensor_osr_t temperatureOSR, bme688_sensor_osr_t pressureOSR, bme688_sensor_osr_t humidityOSR, bme688_i_i_r_filter_coefficient_t filterCoefficient);

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
void bme688_set_heater_config(bme688_config_t *config, uint8_t index, float current, float targetTemp, uint8_t sequences, uint16_t time);

/**
 * @brief Disable BME688 heater
 *
 * @param config BME688 configuration
 */
void bme688_heater_off(bme688_config_t *config);

/**
 * @brief Check if data is ready for reading
 *
 * @param config BME688 configuration
 * @param index Heater profile index
 * @return true if data is ready, false otherwise
 */
bool bme688_is_data_ready(bme688_config_t *config, uint8_t index);

/**
 * @brief Check if data is ready for reading
 *
 * @param config BME688 configuration
 * @param index Heater profile index
 * @return true if data is ready, false otherwise
 */
bool bme688_is_measuring(bme688_config_t *config, uint8_t index);

/**
 * @brief Check if data is ready for reading
 *
 * @param config BME688 configuration
 * @param index Heater profile index
 * @return true if data is ready, false otherwise
 */
bool bme688_is_converting(bme688_config_t *config, uint8_t index);

/**
 * @brief Check if data is ready for reading
 *
 * @param config BME688 configuration
 * @param index Heater profile index
 * @return true if data is ready, false otherwise
 */
uint8_t bme688_get_measurement_index(bme688_config_t *config, uint8_t index);

/**
 * @brief Check if data is ready for reading
 *
 * @param config BME688 configuration
 * @param index Heater profile index
 * @return true if data is ready, false otherwise
 */
bool bme688_is_gas_valid(bme688_config_t *config, uint8_t index);

/**
 * @brief Check if data is ready for reading
 *
 * @param config BME688 configuration
 * @param index Heater profile index
 * @return true if data is ready, false otherwise
 */
bool bme688_is_heater_stable(bme688_config_t *config, uint8_t index);

/**
 * @brief Check if data is ready for reading
 *
 * @param config BME688 configuration
 * @param index Heater profile index
 * @return true if data is ready, false otherwise
 */
uint8_t bme688_get_measurement_sequence_number(bme688_config_t *config, uint8_t index);

#endif