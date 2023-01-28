#include "drivers/accelerometer/bmi088_driver.h"
#include "maths/constants.h"
#include <math.h>
#include "pico/stdlib.h"

FUNCRESULT bmi088AccelInit(BMI088AccelConfig *config, SPIInstance spi, PinNumber miso, PinNumber mosi, PinNumber cs, PinNumber sck)
{
    config->spi = spi;
    config->cs = cs;
    config->rangeConstant = 0.0f;

    spiInitAll(spi, 10000000);
    spiInitPins(spi, miso, mosi, sck, cs);

    __bmi088AccelSetPower(config, TRUE);
    __bmi088AccelSetMode(config, TRUE);

    return SUC_OK;
}

FUNCRESULT bmi088AccelSetConf(BMI088AccelConfig *config, BMI088AccelODR odr, BMI088AccelOSR osr)
{
    BYTE cfg = odr | (osr << 4);

    __bmi088AccelWriteReg(config, ACC_CONF, cfg);

    return SUC_OK;
}

FUNCRESULT bmi088AccelSetRange(BMI088AccelConfig *config, BMI088AccelRange range)
{
    config->rangeConstant = (FLOAT)pow(2, range + 1) * 1.5f * EARTH_GRAVITY;

    __bmi088AccelWriteReg(config, ACC_RANGE, range);

    return SUC_OK;
}

FUNCRESULT bmi088AccelRead(BMI088AccelConfig *config, BMI088AccelData *data)
{
    BYTE buff[6];

    __bmi088AccelReadRegs(config, ACC_DATA, buff, 6);

    INT16 accelX = (buff[1] << 8) | buff[0];
    INT16 accelY = (buff[3] << 8) | buff[2];
    INT16 accelZ = (buff[5] << 8) | buff[4];

    data->accel.x = accelX / 32768.0f * config->rangeConstant;
    data->accel.y = accelY / 32768.0f * config->rangeConstant;
    data->accel.z = accelZ / 32768.0f * config->rangeConstant;

    return SUC_OK;
}

VOID __bmi088AccelSelfTest(BMI088AccelConfig *config)
{
}

VOID __bmi088AccelSoftReset(BMI088AccelConfig *config)
{
    __bmi088AccelWriteReg(config, ACC_SOFTRESET, ACC_SOFTRESET_CMD);

    sleep_ms(50);

    gpioSetPinState(config->cs, GPIO_LOW);
    gpioSetPinState(config->cs, GPIO_HIGH);
}

VOID __bmi088AccelSetMode(BMI088AccelConfig *config, BOOL active)
{
    BYTE data = active ? ACC_PWR_CONF_ACTIVE_CMD : ACC_PWR_CONF_SUSPEND_CMD;

    __bmi088AccelWriteReg(config, ACC_PWR_CONF, data);

    sleep_ms(5);
}

VOID __bmi088AccelSetPower(BMI088AccelConfig *config, BOOL on)
{
    BYTE data = on ? ACC_PWR_CTRL_ON_CMD : ACC_PWR_CTRL_OFF_CMD;

    __bmi088AccelWriteReg(config, ACC_PWR_CTRL, data);

    sleep_ms(5);
}

BYTE __bmi088AccelReadReg(BMI088AccelConfig *config, BYTE address)
{
    BYTE data[2];

    address |= 0x80;

    gpioSetPinState(config->cs, GPIO_LOW);

    spiWriteBlocking(config->spi, &address, 1);
    spiReadBlocking(config->spi, 0, data, 2);

    gpioSetPinState(config->cs, GPIO_HIGH);

    return data[1];
}

VOID __bmi088AccelReadRegs(BMI088AccelConfig *config, BYTE address, BYTE *buffer, SIZE count)
{
    address |= 0x80;

    gpioSetPinState(config->cs, GPIO_LOW);

    spiWriteBlocking(config->spi, &address, 1);
    spiReadBlocking(config->spi, 0, buffer, 1);
    spiReadBlocking(config->spi, 0, buffer, count);

    gpioSetPinState(config->cs, GPIO_HIGH);
}

VOID __bmi088AccelWriteReg(BMI088AccelConfig *config, BYTE address, BYTE data)
{
    address &= 0x7F;

    gpioSetPinState(config->cs, GPIO_LOW);

    spiWriteBlocking(config->spi, &address, 1);
    spiWriteBlocking(config->spi, &data, 1);

    gpioSetPinState(config->cs, GPIO_HIGH);
}

FUNCRESULT bmi088GyroInit(BMI088GyroConfig *config, SPIInstance spi, PinNumber miso, PinNumber mosi, PinNumber cs, PinNumber sck)
{
    config->spi = spi;
    config->cs = cs;
    config->rangeConstant = 0.0f;

    gpioInitPin(cs, GPIO_OUTPUT);
    gpioSetPinState(cs, GPIO_HIGH);
}

FUNCRESULT bmi088GyroSetBandwidth(BMI088GyroConfig *config, BMI088GyroBandwidth bw)
{
    __bmi088GyroWriteReg(config, GYRO_BANDWIDTH, bw);

    return SUC_OK;
}

FUNCRESULT bmi088GyroSetRange(BMI088GyroConfig *config, BYTE range)
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

    __bmi088GyroWriteReg(config, GYRO_RANGE, range);

    return SUC_OK;
}

FUNCRESULT bmi088GyroRead(BMI088GyroConfig *config, BMI088GyroData *data)
{
    BYTE buff[6];

    __bmi088GyroReadRegs(config, GYRO_RATE, buff, 6);

    INT16 gyroX = (buff[1] << 8) | buff[0];
    INT16 gyroY = (buff[3] << 8) | buff[2];
    INT16 gyroZ = (buff[5] << 8) | buff[4];

    data->gyro.x = gyroX / 32767.0f * config->rangeConstant;
    data->gyro.y = gyroY / 32767.0f * config->rangeConstant;
    data->gyro.z = gyroZ / 32767.0f * config->rangeConstant;

    return SUC_OK;
}

VOID __bmi088GyroSelfTest(BMI088GyroConfig *config)
{
}

VOID __bmi088GyroSoftReset(BMI088GyroConfig *config)
{
    __bmi088GyroWriteReg(config, GYRO_SOFTRESET, GYRO_SOFTRESET_CMD);

    sleep_ms(50);

    gpioSetPinState(config->cs, GPIO_LOW);
    gpioSetPinState(config->cs, GPIO_HIGH);
}

BYTE __bmi088GyroReadReg(BMI088GyroConfig *config, BYTE address)
{
    BYTE data[2];

    address |= 0x80;

    gpioSetPinState(config->cs, GPIO_LOW);

    spiWriteBlocking(config->spi, &address, 1);
    spiReadBlocking(config->spi, 0, data, 2);

    gpioSetPinState(config->cs, GPIO_HIGH);

    return data[1];
}

VOID __bmi088GyroReadRegs(BMI088GyroConfig *config, BYTE address, BYTE *buffer, SIZE count)
{
    address |= 0x80;

    gpioSetPinState(config->cs, GPIO_LOW);

    spiWriteBlocking(config->spi, &address, 1);
    spiReadBlocking(config->spi, 0, buffer, 1);
    spiReadBlocking(config->spi, 0, buffer, count);

    gpioSetPinState(config->cs, GPIO_HIGH);
}

VOID __bmi088GyroWriteReg(BMI088GyroConfig *config, BYTE address, BYTE data)
{
    address &= 0x7F;

    gpioSetPinState(config->cs, GPIO_LOW);

    spiWriteBlocking(config->spi, &address, 1);
    spiWriteBlocking(config->spi, &data, 1);

    gpioSetPinState(config->cs, GPIO_HIGH);
}