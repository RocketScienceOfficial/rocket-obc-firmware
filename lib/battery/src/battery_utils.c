#include "lib/battery/battery_utils.h"

void battery_init(battery_config_t *config, battery_interval_t *intervals, uint8_t intervalsCount)
{
    config->intervals = intervals;
    config->intervalsCount = intervalsCount;
}

uint8_t battery_convert(battery_config_t *config, hal_voltage_level_t voltage)
{
    for (uint8_t i = 0; i < config->intervalsCount; i++)
    {
        battery_interval_t *interval = &config->intervals[i];

        if (voltage >= interval->minVolts && voltage <= interval->maxVolts)
        {
            return (uint8_t)(interval->maxPercent - interval->minPercent) / (interval->maxVolts - interval->minVolts) * (voltage - interval->minVolts) + interval->minPercent;
        }
    }

    return 0;
}