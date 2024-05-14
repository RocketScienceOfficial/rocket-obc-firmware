#include "lib/drivers/battery/battery_driver.h"

void battery_init(battery_config_t *config, hal_adc_input_t input, battery_interval_t *intervals, uint8_t intervalsCount)
{
    hal_adc_init_pin(input);

    config->input = input;
    config->intervals = intervals;
    config->intervalsCount = intervalsCount;
}

void battery_read(battery_config_t *config, uint8_t *percent, hal_voltage_level_t *voltage)
{
    *voltage = hal_adc_read_voltage(config->input);
    *percent = battery_convert_voltage_to_percent(config->intervals, config->intervalsCount, *voltage);
}

uint8_t battery_convert_voltage_to_percent(battery_interval_t *intervals, uint8_t intervalsCount, hal_voltage_level_t voltage)
{
    for (uint8_t i = 0; i < intervalsCount; i++)
    {
        if (voltage >= intervals[i].minVolts && voltage <= intervals[i].maxVolts)
        {
            return (uint8_t)(intervals[i].maxPercent - intervals[i].minPercent) / (intervals[i].maxVolts - intervals[i].minVolts) * (voltage - intervals[i].minVolts) + intervals[i].minPercent;
        }
    }

    return 0;
}