#include "drivers/accelerometer/lsm6dso32_driver.h"

#define OUT_TEMP_L 0x20

#define I2C_ADDRESS 0x6A

FUNCRESULT lsm6dso32InitSPI(LSM6DSO32Config *config, SPIInstance spi, PinNumber miso, PinNumber mosi, PinNumber cs, PinNumber sck)
{
    config->gpioConfig = (GPIOCommunicationConfig){
        .spi = spi,
        .cs = cs,
        .readMask = 0x80,
        .multipleReadMask = 0x80,
        .writeMask = 0x7F,
    };

    return spiInitPins(spi, miso, mosi, cs, sck);
}

FUNCRESULT lsm6dso32InitI2C(LSM6DSO32Config *config, I2CInstance i2c, PinNumber sda, PinNumber scl)
{
    config->gpioConfig = (GPIOCommunicationConfig){
        .i2c = i2c,
        .i2cAddress = I2C_ADDRESS,
        .readMask = 0x80,
        .multipleReadMask = 0x80,
        .writeMask = 0x7F,
    };

    return i2cInitPins(i2c, sda, scl);
}

FUNCRESULT lsm6dso32Read(LSM6DSO32Config *config, vec3 *pAcceleration, vec3 *pGyro, FLOAT *pTemperature)
{
    BYTE buffer[14];

    gpioReadRegs(&config->gpioConfig, OUT_TEMP_L, buffer, 14);

    INT16 rawTemperature = (INT16)(buffer[1] << 8) | buffer[0];
    INT16 rawAngularRateX = (INT16)(buffer[3] << 8) | buffer[2];
    INT16 rawAngularRateY = (INT16)(buffer[5] << 8) | buffer[4];
    INT16 rawAngularRateZ = (INT16)(buffer[7] << 8) | buffer[6];
    INT16 rawAccelerationX = (INT16)(buffer[9] << 8) | buffer[8];
    INT16 rawAccelerationY = (INT16)(buffer[11] << 8) | buffer[10];
    INT16 rawAccelerationZ = (INT16)(buffer[13] << 8) | buffer[12];

    *pTemperature = rawTemperature / 32768.0f;
    pGyro->x = rawAngularRateX / 32768.0f * config->gyroRangeConstant;
    pGyro->y = rawAngularRateY / 32768.0f * config->gyroRangeConstant;
    pGyro->z = rawAngularRateZ / 32768.0f * config->gyroRangeConstant;
    pAcceleration->x = rawAccelerationX / 32768.0f * config->accelRangeConstant;
    pAcceleration->y = rawAccelerationY / 32768.0f * config->accelRangeConstant;
    pAcceleration->z = rawAccelerationZ / 32768.0f * config->accelRangeConstant;
}