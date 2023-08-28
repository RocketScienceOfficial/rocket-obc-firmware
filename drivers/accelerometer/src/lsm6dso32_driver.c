#include "drivers/accelerometer/lsm6dso32_driver.h"
#include "maths/constants.h"

#define WHO_AM_I 0x0F
#define CTRL1_XL 0x10
#define CTRL2_G 0x11
#define CTRL3_C 0x12
#define CTRL4_C 0x13
#define CTRL5_C 0x14
#define CTRL6_C 0x15
#define CTRL7_G 0x16
#define CTRL8_XL 0x17
#define CTRL9_XL 0x18
#define CTRL10_C 0x19
#define STATUS_REG 0x1E
#define OUT_TEMP_L 0x20

#define WHO_AM_I_VALUE 0x6C
#define I2C_ADDRESS 0x6A

FUNCRESULT lsm6dso32InitSPI(LSM6DSO32Config *config, SPIInstance spi, PinNumber miso, PinNumber mosi, PinNumber cs, PinNumber sck)
{
    config->gpioConfig = (GPIOCommunicationConfig){
        .protocol = GPIO_PROTOCOL_SPI,
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
        .protocol = GPIO_PROTOCOL_I2C,
        .i2c = i2c,
        .i2cAddress = I2C_ADDRESS,
        .readMask = 0x80,
        .multipleReadMask = 0x80,
        .writeMask = 0x7F,
    };

    return i2cInitPins(i2c, sda, scl);
}

FUNCRESULT lsm6dso32ValidateId(LSM6DSO32Config *config, BOOL *valid)
{
    *valid = gpioReadReg(&config->gpioConfig, WHO_AM_I) == WHO_AM_I_VALUE;

    return SUC_OK;
}

FUNCRESULT lsm6dso32SetODR(LSM6DSO32Config *config, LSM6DSO32ODR accelODR, LSM6DSO32ODR gyroODR)
{
    gpioWriteRegField(&config->gpioConfig, CTRL1_XL, 4, 4, (BYTE)accelODR);
    gpioWriteRegField(&config->gpioConfig, CTRL2_G, 4, 4, (BYTE)gyroODR);

    return SUC_OK;
}

FUNCRESULT lsm6dso32SetRange(LSM6DSO32Config *config, LSM6DSO32AccelRange accelRange, LSM6DSO32GyroRange gyroRange)
{
    FLOAT acc = 0.0f;
    FLOAT gyr = 0.0f;

    if (accelRange == LSM6DSO32_RANGE_4G)
        acc = 4.0f;
    else if (accelRange == LSM6DSO32_RANGE_8G)
        acc = 8.0f;
    else if (accelRange == LSM6DSO32_RANGE_16G)
        acc = 16.0f;
    else if (accelRange == LSM6DSO32_RANGE_32G)
        acc = 32.0f;

    if (gyroRange == LSM6DSO32_RANGE_125DPS)
        gyr = 125.0f;
    else if (gyroRange == LSM6DSO32_RANGE_250DPS)
        gyr = 250.0f;
    else if (gyroRange == LSM6DSO32_RANGE_500DPS)
        gyr = 500.0f;
    else if (gyroRange == LSM6DSO32_RANGE_1000DPS)
        gyr = 1000.0f;
    else if (gyroRange == LSM6DSO32_RANGE_2000DPS)
        gyr = 2000.0f;

    config->accelRangeConstant = acc * EARTH_GRAVITY;
    config->gyroRangeConstant = DEG_2_RAD(gyr);

    gpioWriteRegField(&config->gpioConfig, CTRL1_XL, 2, 2, (BYTE)accelRange);

    if (gyroRange != LSM6DSO32_RANGE_125DPS)
    {
        gpioWriteRegField(&config->gpioConfig, CTRL7_G, 1, 1, 0x00);
        gpioWriteRegField(&config->gpioConfig, CTRL2_G, 2, 2, (BYTE)gyroRange);
    }
    else
    {
        gpioWriteRegField(&config->gpioConfig, CTRL7_G, 1, 1, 0x01);
    }

    return SUC_OK;
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

    if (pTemperature != NULL)
    {
        *pTemperature = rawTemperature / 32768.0f;
    }

    pGyro->x = rawAngularRateX / 32768.0f * config->gyroRangeConstant;
    pGyro->y = rawAngularRateY / 32768.0f * config->gyroRangeConstant;
    pGyro->z = rawAngularRateZ / 32768.0f * config->gyroRangeConstant;
    pAcceleration->x = rawAccelerationX / 32768.0f * config->accelRangeConstant;
    pAcceleration->y = rawAccelerationY / 32768.0f * config->accelRangeConstant;
    pAcceleration->z = rawAccelerationZ / 32768.0f * config->accelRangeConstant;
}