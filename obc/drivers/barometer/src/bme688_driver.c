#include "drivers/barometer/bme688_driver.h"

#define STATUS 0x73
#define VARIANT_ID 0x70
#define RESET 0x60
#define CHIP_ID 0x50
#define CONFIG 0x75
#define CTRL_MEAS 0x74
#define CTRL_HUM 0x72
#define CTRL_GAS_1 0x71
#define CTRL_GAS_0 0x70
#define GAS_WAIT_SHARED 0x6E
#define GAS_WAIT_X 0x64
#define RES_HEAT_X 0x5A
#define IDAC_HEAT_X 0x50
#define SUB_MEAS_INDEX_0 0x1E
#define SUB_MEAS_INDEX_1 0x2F
#define MEAS_STATUS_0 0x1D
#define MEAS_STATUS_1 0x2E
#define PRESS_MSB_0 0x1F
#define PRESS_MSB_1 0x30
#define PRESS_MSB_2 0x41
#define PRESS_LSB_0 0x20
#define PRESS_LSB_1 0x31
#define PRESS_LSB_2 0x42
#define PRESS_XLSB_0 0x21
#define PRESS_XLSB_1 0x32
#define PRESS_XLSB_2 0x43
#define TEMP_MSB_0 0x22
#define TEMP_MSB_1 0x33
#define TEMP_MSB_2 0x44
#define TEMP_LSB_0 0x23
#define TEMP_LSB_1 0x34
#define TEMP_LSB_2 0x45
#define TEMP_XLSB_0 0x24
#define TEMP_XLSB_1 0x35
#define TEMP_XLSB_2 0x46
#define HUM_MSB_0 0x25
#define HUM_MSB_1 0x36
#define HUM_MSB_2 0x47
#define HUM_LSB_0 0x26
#define HUM_LSB_1 0x37
#define HUM_LSB_2 0x48
#define GAS_R_MSB_0 0x2C
#define GAS_R_MSB_1 0x3D
#define GAS_R_MSB_2 0x4E
#define GAS_R_LSB_0 0x2D
#define GAS_R_LSB_1 0x3E
#define GAS_R_LSB_2 0x4F

#define SOFT_RESET_CMD 0xB6

FUNCRESULT bme688Init(BME688Config *config, SPIInstance spi, PinNumber miso, PinNumber mosi, PinNumber sck, PinNumber cs)
{
    config->spi = spi;
    config->cs = cs;

    spiInitPins(spi, miso, mosi, sck, cs);

    return SUC_OK;
}

FUNCRESULT bme688Read(BME688Config *config, BME688Data *data)
{
    return SUC_OK;
}

FUNCRESULT bme688GetVariantId(BME688Config *config, BYTE *variantId)
{
    *variantId = __bme688ReadReg(config, VARIANT_ID);

    return SUC_OK;
}

FUNCRESULT bme688GetChipId(BME688Config *config, BYTE *chipId)
{
    *chipId = __bme688ReadReg(config, CHIP_ID);

    return SUC_OK;
}

FUNCRESULT bme688SetPowerMode(BME688Config *config, BME688PowerMode mode)
{
    __bme688WriteReg(config, CTRL_MEAS, mode);

    return SUC_OK;
}

FUNCRESULT bme688SetHumidityOSR(BME688Config *config, BME688SensorOSR osr)
{
    __bme688WriteReg(config, CTRL_HUM, osr);

    return SUC_OK;
}

FUNCRESULT bme688SetTemperatureOSR(BME688Config *config, BME688SensorOSR osr)
{
    BYTE ctrl_meas = __bme688ReadReg(config, CTRL_MEAS);

    ctrl_meas &= 0x1F;
    ctrl_meas |= osr << 5;

    __bme688WriteReg(config, CTRL_MEAS, ctrl_meas);

    return SUC_OK;
}

FUNCRESULT bme688SetPressureOSR(BME688Config *config, BME688SensorOSR osr)
{
    BYTE ctrl_meas = __bme688ReadReg(config, CTRL_MEAS);

    ctrl_meas &= 0xE3;
    ctrl_meas |= osr << 2;

    __bme688WriteReg(config, CTRL_MEAS, ctrl_meas);

    return SUC_OK;
}

FUNCRESULT bme688SetIIRFilter(BME688Config *config, BME688IIRFilterCoefficient filter)
{
    BYTE cfg = __bme688ReadReg(config, CONFIG);

    cfg &= 0xE3;
    cfg |= filter << 2;

    __bme688WriteReg(config, CONFIG, cfg);

    return SUC_OK;
}

VOID __bme688SetPage(BME688Config *config, BYTE page)
{
    BYTE status = __bme688ReadReg(config, STATUS);

    page << 4;
    status |= page;

    __bme688WriteReg(config, STATUS, status);
}

VOID __bme688SoftReset(BME688Config *config)
{
    __bme688WriteReg(config, RESET, SOFT_RESET_CMD);
}

BYTE __bme688ReadReg(BME688Config *config, BYTE address)
{
    BYTE data;

    address |= 0x80;

    gpioSetPinState(config->cs, GPIO_LOW);

    spiWriteBlocking(config->spi, &address, 1);
    spiReadBlocking(config->spi, 0, &data, 1);

    gpioSetPinState(config->cs, GPIO_HIGH);

    return data;
}

VOID __bme688ReadRegs(BME688Config *config, BYTE address, BYTE *buffer, SIZE count)
{
    address |= 0x80;

    gpioSetPinState(config->cs, GPIO_LOW);

    spiWriteBlocking(config->spi, &address, 1);
    spiReadBlocking(config->spi, 0, buffer, count);

    gpioSetPinState(config->cs, GPIO_HIGH);
}

VOID __bme688WriteReg(BME688Config *config, BYTE address, BYTE data)
{
    address &= 0x7F;

    gpioSetPinState(config->cs, GPIO_LOW);

    spiWriteBlocking(config->spi, &address, 1);
    spiReadBlocking(config->spi, 0, &data, 1);

    gpioSetPinState(config->cs, GPIO_HIGH);
}