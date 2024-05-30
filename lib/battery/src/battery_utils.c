#include "lib/battery/battery_utils.h"
#include <math.h>

void battery_init(battery_config_t *config, battery_table_entry_t *entries, uint8_t entriesCount, hal_voltage_level_t voltageDivider)
{
    if (entriesCount >= 2)
    {
        config->entries = entries;
        config->entriesCount = entriesCount;
        config->voltageDivider = voltageDivider;
        config->oneCellMaxVoltage = entries[0].voltage;
    }
}

void battery_convert(const battery_config_t *config, hal_voltage_level_t rawVoltage, battery_data_t *data)
{
    if (config->entries)
    {
        data->voltage = rawVoltage * config->voltageDivider;
        data->nCells = (uint8_t)ceilf(data->voltage / config->oneCellMaxVoltage);

        hal_voltage_level_t oneCellVoltage = data->voltage / data->nCells;

        for (uint8_t i = 1; i < config->entriesCount; i++)
        {
            if (oneCellVoltage >= config->entries[i].voltage && oneCellVoltage <= config->entries[i - 1].voltage)
            {
                data->percentage = (uint8_t)(config->entries[i - 1].percentage - config->entries[i].percentage) / (config->entries[i - 1].voltage - config->entries[i].voltage) * (oneCellVoltage - config->entries[i].voltage) + config->entries[i].percentage;
            }
            else if (i == config->entriesCount - 1)
            {
                data->percentage = 0;
            }
        }
    }
}