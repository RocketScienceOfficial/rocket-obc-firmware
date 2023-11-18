#include "modules/drivers/gps/u_blox_sam_m10q_driver.h"
#include <string.h>

#define I2C_ADDRESS 0x42

void ublox_sam_m10q_init_spi(ublox_sam_m10q_config_t *config, spi_instance_t spi, pin_number_t miso, pin_number_t mosi, pin_number_t cs, pin_number_t sck)
{
    config->gpioConfig = (gpio_communication_config_t){
        .protocol = GPIO_PROTOCOL_SPI,
        .spi = spi,
        .cs = cs,
    };

    spi_init_pins(spi, miso, mosi, sck, cs);
}

void ublox_sam_m10q_init_i2c(ublox_sam_m10q_config_t *config, i2c_instance_t i2c, pin_number_t sda, pin_number_t scl)
{
    config->gpioConfig = (gpio_communication_config_t){
        .protocol = GPIO_PROTOCOL_I2C,
        .i2c = i2c,
        .i2cAddress = I2C_ADDRESS,
    };

    i2c_init_pins(i2c, sda, scl);
}

void ublox_sam_m10q_read_data(ublox_sam_m10q_config_t *config, ublox_sam_m10q_data_t *data)
{
    uint8_t b = gpio_single_read(&config->gpioConfig);

    if (b == 0xFF)
    {
        return;
    }

    if (b == '$')
    {
        data->isValid = true;
        data->currentIndex = 0;
        data->isFinishedSentence = false;

        memset(data->sentence, 0, UBLOX_SAM_M10Q_SENTENCE_LENGTH);
    }
    else if (b == '\n')
    {
        data->sentence[data->currentIndex] = b;
        data->sentence[data->currentIndex + 1] = '\0';
        data->isValid = false;
        data->isFinishedSentence = true;
    }

    if (data->isValid)
    {
        data->sentence[data->currentIndex] = b;
        data->currentIndex++;
    }
}