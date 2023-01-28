#pragma once

#include <obc/api.h>
#include "drivers/gpio/spi_driver.h"
#include "drivers/gpio/gpio_driver.h"
#include "maths/vector.h"

#define ACC_CHIP_ID 0x00
#define ACC_ERR_REG 0x02
#define ACC_STATUS 0x03
#define ACC_DATA 0x12
#define ACC_SENSORTIME 0x18
#define ACC_INT_STAT_1 0x1D
#define ACC_TEMP 0x22
#define ACC_FIFO_LENGTH 0x24
#define ACC_FIFO_DATA 0x26
#define ACC_CONF 0x40
#define ACC_OSR_NORMAL 0x0A
#define ACC_OSR_2 0x09
#define ACC_OSR_4 0x08
#define ACC_RANGE 0x41
#define ACC_FIFO_DOWNS 0x45
#define ACC_FIFO_WTM 0x46
#define ACC_FIFO_CONFIG 0x48
#define ACC_INT1_IO_CTRL 0x53
#define ACC_INT2_IO_CTRL 0x54
#define ACC_INT_MAP 0x58
#define ACC_SELF_TEST 0x6D
#define ACC_SELF_TEST_OFF_CMD 0x00
#define ACC_SELF_TEST_POS_SIG_CMD 0x0D
#define ACC_SELF_TEST_NEG_SIG_CMD 0x09
#define ACC_PWR_CONF 0x7C
#define ACC_PWR_CONF_SUSPEND_CMD 0x03
#define ACC_PWR_CONF_ACTIVE_CMD 0x00
#define ACC_PWR_CTRL 0x7D
#define ACC_PWR_CTRL_OFF_CMD 0x00
#define ACC_PWR_CTRL_ON_CMD 0x04
#define ACC_SOFTRESET 0x7F
#define ACC_SOFTRESET_CMD 0xB6

#define GYRO_CHIP_ID 0x00
#define GYRO_RATE 0x02
#define GYRO_INT_STAT_1 0x0A
#define GYRO_FIFO_STATUS 0x0E
#define GYRO_RANGE 0x0F
#define GYRO_BANDWIDTH 0x10
#define GYRO_LPM1 0x11
#define GYRO_LPM1_NORMAL_CMD 0x00
#define GYRO_LPM1_SUSPEND_CMD 0x80
#define GYRO_LPM1_DEEP_SUSPEND_CMD 0x20
#define GYRO_SOFTRESET 0x14
#define GYRO_SOFTRESET_CMD 0xB6
#define GYRO_INT_CTRL 0x15
#define GYRO_INT3_INT4_IO_CONF 0x16
#define GYRO_INT3_INT4_IO_MAP 0x18
#define GYRO_WM_EN 0x1E
#define FIFO_EXT_INT_S 0x34
#define GYRO_SELF_TEST 0x3C
#define GYRO_FIFO_CONFIG 0x3D
#define GYRO_FIFO_DATA 0x3E

typedef struct BMI088AccelConfig
{
    SPIInstance spi;
    PinNumber cs;
    FLOAT rangeConstant;
} BMI088AccelConfig;

typedef struct BMI088AccelData
{
    vec3 accel;
} BMI088AccelData;

typedef enum BMI088AccelRange
{
    BMI088_ACCEL_RANGE_2G = 0x00,
    BMI088_ACCEL_RANGE_4G = 0x01,
    BMI088_ACCEL_RANGE_8G = 0x02,
    BMI088_ACCEL_RANGE_16G = 0x03,
} BMI088AccelRange;

typedef enum BMI088AccelODR
{
    BMI088_ACCEL_ODR_12_5HZ = 0x05,
    BMI088_ACCEL_ODR_25HZ = 0x06,
    BMI088_ACCEL_ODR_50HZ = 0x07,
    BMI088_ACCEL_ODR_100HZ = 0x08,
    BMI088_ACCEL_ODR_200HZ = 0x09,
    BMI088_ACCEL_ODR_400HZ = 0x0A,
    BMI088_ACCEL_ODR_800HZ = 0x0B,
    BMI088_ACCEL_ODR_1600HZ = 0x0C,
} BMI088AccelODR;

typedef enum BMI088AccelOSR
{
    BMI088_ACCEL_OSR_NORMAL = 0x0A,
    BMI088_ACCEL_OSR_2 = 0x09,
    BMI088_ACCEL_OSR_4 = 0x08,
} BMI088AccelOSR;

typedef struct BMI088GyroConfig
{
    SPIInstance spi;
    PinNumber cs;
    FLOAT rangeConstant;
} BMI088GyroConfig;

typedef struct BMI088GyroData
{
    vec3 gyro;
} BMI088GyroData;

typedef enum BMI088GyroRange
{
    BMI088_GYRO_RANGE_2000DPS = 0x00,
    BMI088_GYRO_RANGE_1000DPS = 0x01,
    BMI088_GYRO_RANGE_500DPS = 0x02,
    BMI088_GYRO_RANGE_250DPS = 0x03,
    BMI088_GYRO_RANGE_125DPS = 0x04,
} BMI088GyroRange;

typedef enum BMI088GyroBandwidth
{
    BMI088_GYRO_ODR_2000_BW_523HZ = 0x00,
    BMI088_GYRO_ODR_2000_BW_230HZ = 0x01,
    BMI088_GYRO_ODR_1000_BW_116HZ = 0x02,
    BMI088_GYRO_ODR_400_BW_47HZ = 0x03,
    BMI088_GYRO_ODR_200_BW_23HZ = 0x04,
    BMI088_GYRO_ODR_100_BW_12HZ = 0x05,
    BMI088_GYRO_ODR_200_BW_64HZ = 0x06,
    BMI088_GYRO_ODR_100_BW_32HZ = 0x07,
} BMI088GyroBandwidth;

FUNCRESULT bmi088AccelInit(BMI088AccelConfig *config, SPIInstance spi, PinNumber miso, PinNumber mosi, PinNumber cs, PinNumber sck);
FUNCRESULT bmi088AccelSetConf(BMI088AccelConfig *config, BMI088AccelODR odr, BMI088AccelOSR osr);
FUNCRESULT bmi088AccelSetRange(BMI088AccelConfig *config, BMI088AccelRange range);
FUNCRESULT bmi088AccelRead(BMI088AccelConfig *config, BMI088AccelData *data);
VOID __bmi088AccelSelfTest(BMI088AccelConfig *config);
VOID __bmi088AccelSoftReset(BMI088AccelConfig *config);
VOID __bmi088AccelSetMode(BMI088AccelConfig *config, BOOL active);
VOID __bmi088AccelSetPower(BMI088AccelConfig *config, BOOL on);
BYTE __bmi088AccelReadReg(BMI088AccelConfig *config, BYTE address);
VOID __bmi088AccelReadRegs(BMI088AccelConfig *config, BYTE address, BYTE *buffer, SIZE count);
VOID __bmi088AccelWriteReg(BMI088AccelConfig *config, BYTE address, BYTE data);

FUNCRESULT bmi088GyroInit(BMI088GyroConfig *config, SPIInstance spi, PinNumber miso, PinNumber mosi, PinNumber cs, PinNumber sck);
FUNCRESULT bmi088GyroSetBandwidth(BMI088GyroConfig *config, BMI088GyroBandwidth bw);
FUNCRESULT bmi088GyroSetRange(BMI088GyroConfig *config, BYTE range);
FUNCRESULT bmi088GyroRead(BMI088GyroConfig *config, BMI088GyroData *data);
VOID __bmi088GyroSelfTest(BMI088GyroConfig *config);
VOID __bmi088GyroSoftReset(BMI088GyroConfig *config);
BYTE __bmi088GyroReadReg(BMI088GyroConfig *config, BYTE address);
VOID __bmi088GyroReadRegs(BMI088GyroConfig *config, BYTE address, BYTE *buffer, SIZE count);
VOID __bmi088GyroWriteReg(BMI088GyroConfig *config, BYTE address, BYTE data);