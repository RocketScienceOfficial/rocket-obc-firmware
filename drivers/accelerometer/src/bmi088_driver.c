#include "drivers/accelerometer/bmi088_driver.h"
#include "drivers/tools/time_tracker.h"
#include "maths/constants.h"
#include <math.h>
#include <string.h>

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

FUNCRESULT bmi088AccelInitSPI(BMI088AccelConfig *config, SPIInstance spi, PinNumber miso, PinNumber mosi, PinNumber cs, PinNumber sck)
{
    config->gpioConfig = (GPIOCommunicationConfig){
        .protocol = GPIO_PROTOCOL_SPI,
        .spi = spi,
        .cs = cs,
        .readMask = 0x80,
        .multipleReadMask = 0x80,
        .writeMask = 0x7F,
    };
    config->rangeConstant = 0.0f;

    spiInitPins(spi, miso, mosi, sck, cs);

    __bmi088InitBase(config);

    return SUC_OK;
}

FUNCRESULT bmi088AccelInitI2C(BMI088AccelConfig *config, I2CInstance i2c, PinNumber sda, PinNumber scl, BYTE address)
{
    config->gpioConfig = (GPIOCommunicationConfig){
        .protocol = GPIO_PROTOCOL_SPI,
        .i2c = i2c,
        .i2cAddress = address,
        .readMask = 0x80,
        .multipleReadMask = 0x80,
        .writeMask = 0x7F,
    };
    config->rangeConstant = 0.0f;

    i2cInitPins(i2c, sda, scl);

    __bmi088InitBase(config);

    return SUC_OK;
}

FUNCRESULT bmi088AccelSetConf(BMI088AccelConfig *config, BMI088AccelODR odr, BMI088AccelOSR osr)
{
    BYTE cfg = odr | (osr << 4);

    __bmi088AccelWriteReg(config, ACC_CONF, cfg);

    sleepMiliseconds(5);

    return SUC_OK;
}

FUNCRESULT bmi088AccelSetRange(BMI088AccelConfig *config, BMI088AccelRange range)
{
    config->rangeConstant = (FLOAT)pow(2, range + 1) * 1.5f * EARTH_GRAVITY;

    __bmi088AccelWriteReg(config, ACC_RANGE, range);

    sleepMiliseconds(5);

    return SUC_OK;
}

FUNCRESULT bmi088AccelRead(BMI088AccelConfig *config, vec3 *accel)
{
    BYTE buff[6];

    __bmi088AccelReadRegs(config, ACC_DATA, buff, 6);

    INT16 accelX = (INT16)((buff[1] << 8) | buff[0]);
    INT16 accelY = (INT16)((buff[3] << 8) | buff[2]);
    INT16 accelZ = (INT16)((buff[5] << 8) | buff[4]);

    accel->x = accelX / 32768.0f * config->rangeConstant;
    accel->y = accelY / 32768.0f * config->rangeConstant;
    accel->z = accelZ / 32768.0f * config->rangeConstant;

    return SUC_OK;
}

VOID __bmi088AccelSoftReset(BMI088AccelConfig *config)
{
    __bmi088AccelWriteReg(config, ACC_SOFTRESET, ACC_SOFTRESET_CMD);

    sleepMiliseconds(50);
}

VOID __bmi088AccelSetMode(BMI088AccelConfig *config, BOOL active)
{
    BYTE data = active ? ACC_PWR_CONF_ACTIVE_CMD : ACC_PWR_CONF_SUSPEND_CMD;

    __bmi088AccelWriteReg(config, ACC_PWR_CONF, data);

    sleepMiliseconds(5);
}

VOID __bmi088AccelSetPower(BMI088AccelConfig *config, BOOL on)
{
    BYTE data = on ? ACC_PWR_CTRL_ON_CMD : ACC_PWR_CTRL_OFF_CMD;

    __bmi088AccelWriteReg(config, ACC_PWR_CTRL, data);

    sleepMiliseconds(5);
}

VOID __bmi088InitBase(BMI088AccelConfig *config)
{
    __bmi088AccelSetPower(config, TRUE);
    __bmi088AccelSetMode(config, TRUE);
}

BYTE __bmi088AccelReadReg(BMI088AccelConfig *config, BYTE address)
{
    BYTE data[2];

    gpioReadRegs(&config->gpioConfig, address, data, 2);

    return data[1];
}

VOID __bmi088AccelReadRegs(BMI088AccelConfig *config, BYTE address, BYTE *buffer, SIZE count)
{
    BYTE tmp_buffer[16];

    gpioReadRegs(&config->gpioConfig, address, tmp_buffer, count + 1);

    memcpy(buffer, tmp_buffer + 1, count);
}

VOID __bmi088AccelWriteReg(BMI088AccelConfig *config, BYTE address, BYTE data)
{
    gpioWriteReg(&config->gpioConfig, address, data);
}

FUNCRESULT bmi088GyroInitSPI(BMI088GyroConfig *config, SPIInstance spi, PinNumber miso, PinNumber mosi, PinNumber cs, PinNumber sck)
{
    config->gpioConfig = (GPIOCommunicationConfig){
        .protocol = GPIO_PROTOCOL_SPI,
        .spi = spi,
        .cs = cs,
        .readMask = 0x80,
        .multipleReadMask = 0x80,
        .writeMask = 0x7F,
    };
    config->rangeConstant = 0.0f;

    return spiInitPins(spi, miso, mosi, sck, cs);
}

FUNCRESULT bmi088GyroInitI2C(BMI088GyroConfig *config, I2CInstance i2c, PinNumber sda, PinNumber scl, BYTE address)
{
    config->gpioConfig = (GPIOCommunicationConfig){
        .protocol = GPIO_PROTOCOL_I2C,
        .i2c = i2c,
        .i2cAddress = address,
        .readMask = 0x80,
        .multipleReadMask = 0x80,
        .writeMask = 0x7F,
    };
    config->rangeConstant = 0.0f;

    return i2cInitPins(i2c, sda, scl);
}

FUNCRESULT bmi088GyroSetBandwidth(BMI088GyroConfig *config, BMI088GyroBandwidth bw)
{
    gpioWriteReg(&config->gpioConfig, GYRO_BANDWIDTH, (BYTE)bw | 0x80);

    sleepMiliseconds(5);

    return SUC_OK;
}

FUNCRESULT bmi088GyroSetRange(BMI088GyroConfig *config, BMI088GyroRange range)
{
    FLOAT val = 0.0f;

    if (range == BMI088_GYRO_RANGE_125DPS)
        val = 125.0f;
    else if (range == BMI088_GYRO_RANGE_250DPS)
        val = 250.0f;
    else if (range == BMI088_GYRO_RANGE_500DPS)
        val = 500.0f;
    else if (range == BMI088_GYRO_RANGE_1000DPS)
        val = 1000.0f;
    else if (range == BMI088_GYRO_RANGE_2000DPS)
        val = 2000.0f;

    config->rangeConstant = DEG_2_RAD(val);

    gpioWriteReg(&config->gpioConfig, GYRO_RANGE, range);

    sleepMiliseconds(5);

    return SUC_OK;
}

FUNCRESULT bmi088GyroRead(BMI088GyroConfig *config, vec3 *gyro)
{
    BYTE buff[6];

    gpioReadRegs(&config->gpioConfig, GYRO_RATE, buff, 6);

    INT16 gyroX = (INT16)((buff[1] << 8) | buff[0]);
    INT16 gyroY = (INT16)((buff[3] << 8) | buff[2]);
    INT16 gyroZ = (INT16)((buff[5] << 8) | buff[4]);

    gyro->x = gyroX / 32767.0f * config->rangeConstant;
    gyro->y = gyroY / 32767.0f * config->rangeConstant;
    gyro->z = gyroZ / 32767.0f * config->rangeConstant;

    return SUC_OK;
}

VOID __bmi088GyroSoftReset(BMI088GyroConfig *config)
{
    gpioWriteReg(&config->gpioConfig, GYRO_SOFTRESET, GYRO_SOFTRESET_CMD);

    sleepMiliseconds(50);
}