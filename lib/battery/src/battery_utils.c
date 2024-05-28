#include "lib/battery/battery_utils.h"
#include <math.h>

void battery_init(battery_config_t *config, battery_interval_t *intervals, uint8_t intervalsCount, hal_voltage_level_t voltageDivider)
{
    config->intervals = intervals;
    config->intervalsCount = intervalsCount;
    config->voltageDivider = voltageDivider;
    config->oneCellMaxVoltage = 0;

    for (uint8_t i = 0; i < intervalsCount; i++)
    {
        if (intervals[i].maxVolts > config->oneCellMaxVoltage)
        {
            config->oneCellMaxVoltage = intervals[i].maxVolts;
        }
    }
}

void battery_convert(const battery_config_t *config, hal_voltage_level_t rawVoltage, battery_data_t *data)
{
    data->voltage = rawVoltage * config->voltageDivider;
    data->nCells = (uint8_t)ceil(data->voltage / config->oneCellMaxVoltage);

    hal_voltage_level_t oneCellVoltage = data->voltage / data->nCells;

    for (uint8_t i = 0; i < config->intervalsCount; i++)
    {
        battery_interval_t *interval = &config->intervals[i];

        if (oneCellVoltage >= interval->minVolts && oneCellVoltage <= interval->maxVolts)
        {
            data->percentage = (uint8_t)(interval->maxPercent - interval->minPercent) / (interval->maxVolts - interval->minVolts) * (oneCellVoltage - interval->minVolts) + interval->minPercent;
        }
        else if (i == config->intervalsCount - 1)
        {
            data->percentage = 0;
        }
    }
}