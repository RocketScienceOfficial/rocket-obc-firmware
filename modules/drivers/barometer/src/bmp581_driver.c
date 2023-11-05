#include "drivers/barometer/bmp581_driver.h"
#include "drivers/tools/time_tracker.h"

#define CHIP_ID 0x01
#define REV_ID 0x02
#define CHIP_STATUS 0x11
#define DRIVE_CONFIG 0x1B
#define INT_CONFIG 0x14
#define INT_SOURCE 0x15
#define FIFO_CONFIG 0x16
#define FIFO_COUNT 0x17
#define FIFO_SEL 0x18
#define TEMP_DATA_XLSB 0x1D
#define TEMP_DATA_LSB 0x1E
#define TEMP_DATA_MSB 0x1F
#define PRESS_DATA_XLSB 0x20
#define PRESS_DATA_LSB 0x21
#define PRESS_DATA_MSB 0x22
#define INT_STATUS 0x27
#define STATUS 0x28
#define FIFO_DATA 0x29
#define NVM_ADDR 0x2B
#define NVM_DATA_LSB 0x2C
#define NVM_DATA_MSB 0x2D
#define DSP_CONFIG 0x30
#define DSP_IIR 0x31
#define OOR_THR_P_LSB 0x32
#define OOR_THR_P_MSB 0x33
#define OOR_RANGE 0x34
#define OOR_CONFIG 0x35
#define OSR_CONFIG 0x36
#define ODR_CONFIG 0x37
#define OSR_EFF 0x38
#define CMD 0x7E

#define I2C_ADDRESS 0x46

FUNCRESULT bmp581InitSPI(BMP581Config *config, SPIInstance spi, PinNumber miso, PinNumber mosi, PinNumber cs, PinNumber sck)
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

FUNCRESULT bmp581InitI2C(BMP581Config *config, I2CInstance i2c, PinNumber sda, PinNumber scl)
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

FUNCRESULT bmp581ValidateId(BMP581Config *config, BOOL *pValid)
{
    BYTE buffer[2];

    gpioReadRegs(&config->gpioConfig, CHIP_ID, buffer, 2);

    *pValid = buffer[0] == 0x58 && buffer[1] == 0x01;

    return SUC_OK;
}

FUNCRESULT bmp581SetODR_OSR(BMP581Config *config, BMP581OSR tempOSR, BMP581OSR pressOSR, BMP581ODR odr)
{
    gpioWriteReg(&config->gpioConfig, OSR_CONFIG, ((BYTE)pressOSR << 3) | (BYTE)tempOSR | 1 << 6);
    gpioWriteRegField(&config->gpioConfig, ODR_CONFIG, 5, 2, (BYTE)odr);

    return SUC_OK;
}

FUNCRESULT bmp581SetMode(BMP581Config *config, BMP581Mode mode)
{
    gpioWriteRegField(&config->gpioConfig, ODR_CONFIG, 2, 0, (BYTE)mode);

    return SUC_OK;
}

FUNCRESULT bmp581Read(BMP581Config *config, BMP581Data *data)
{
    BYTE buffer[6];

    gpioReadRegs(&config->gpioConfig, TEMP_DATA_XLSB, buffer, 6);

    INT32 raw_temp = (INT32)buffer[2] << 16 | (INT32)buffer[1] << 8 | (INT32)buffer[0];
    INT32 raw_press = (INT32)buffer[5] << 16 | (INT32)buffer[4] << 8 | (INT32)buffer[3];

    data->temperature = (FLOAT)raw_temp / 65536.0f;
    data->pressure = (FLOAT)raw_press / 64.0f;

    return SUC_OK;
}

VOID __bmp581SoftReset(BMP581Config *config)
{
    gpioWriteReg(&config->gpioConfig, CMD, 0xB6);

    sleepMiliseconds(10);
}