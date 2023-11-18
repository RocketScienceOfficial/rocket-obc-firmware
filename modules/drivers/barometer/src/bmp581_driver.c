#include "modules/drivers/barometer/bmp581_driver.h"
#include "modules/drivers/hal/time_tracker.h"

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

void bmp581_init_spi(bmp581_config_t *config, spi_instance_t spi, pin_number_t miso, pin_number_t mosi, pin_number_t cs, pin_number_t sck)
{
    config->gpioConfig = (gpio_communication_config_t){
        .protocol = GPIO_PROTOCOL_SPI,
        .spi = spi,
        .cs = cs,
        .readMask = 0x80,
        .multipleReadMask = 0x80,
        .writeMask = 0x7F,
    };

    return spi_init_pins(spi, miso, mosi, cs, sck);
}

void bmp581_init_i2c(bmp581_config_t *config, i2c_instance_t i2c, pin_number_t sda, pin_number_t scl)
{
    config->gpioConfig = (gpio_communication_config_t){
        .protocol = GPIO_PROTOCOL_I2C,
        .i2c = i2c,
        .i2cAddress = I2C_ADDRESS,
        .readMask = 0x80,
        .multipleReadMask = 0x80,
        .writeMask = 0x7F,
    };

    return i2c_init_pins(i2c, sda, scl);
}

void bmp581_validate_id(bmp581_config_t *config, bool *pValid)
{
    uint8_t buffer[2];

    gpio_read_regs(&config->gpioConfig, CHIP_ID, buffer, 2);

    *pValid = buffer[0] == 0x58 && buffer[1] == 0x01;
}

void bmp581_set_odr__osr(bmp581_config_t *config, bmp581_osr_t tempOSR, bmp581_osr_t pressOSR, bmp581_odr_t odr)
{
    gpio_write_reg(&config->gpioConfig, OSR_CONFIG, ((uint8_t)pressOSR << 3) | (uint8_t)tempOSR | 1 << 6);
    gpio_write_reg_field(&config->gpioConfig, ODR_CONFIG, 5, 2, (uint8_t)odr);
}

void bmp581_set_mode(bmp581_config_t *config, bmp581_mode_t mode)
{
    gpio_write_reg_field(&config->gpioConfig, ODR_CONFIG, 2, 0, (uint8_t)mode);
}

void bmp581_read(bmp581_config_t *config, bmp581_data_t *data)
{
    uint8_t buffer[6];

    gpio_read_regs(&config->gpioConfig, TEMP_DATA_XLSB, buffer, 6);

    int raw_temp = (int)buffer[2] << 16 | (int)buffer[1] << 8 | (int)buffer[0];
    int raw_press = (int)buffer[5] << 16 | (int)buffer[4] << 8 | (int)buffer[3];

    data->temperature = (float)raw_temp / 65536.0f;
    data->pressure = (float)raw_press / 64.0f;
}

void _bmp581_soft_reset(bmp581_config_t *config)
{
    gpio_write_reg(&config->gpioConfig, CMD, 0xB6);

    time_sleep_ms(10);
}