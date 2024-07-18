#include "lib/battery/battery_utils.h"
#include <math.h>

void battery_init(battery_config_t *config, battery_table_entry_t *entries, uint8_t entriesCount)
{
    if (entriesCount >= 2)
    {
        config->entries = entries;
        config->entriesCount = entriesCount;
        config->oneCellMinVoltage = entries[entriesCount - 1].voltage;
    }
}

void battery_convert(const battery_config_t *config, float voltage, battery_data_t *data)
{
    if (config->entries)
    {
        data->nCells = (uint8_t)floorf(voltage / config->oneCellMinVoltage);

        if (data->nCells > 0)
        {
            float oneCellVoltage = voltage / data->nCells;

            for (uint8_t i = 1; i < config->entriesCount; i++)
            {
                if (oneCellVoltage >= config->entries[i].voltage && oneCellVoltage <= config->entries[i - 1].voltage)
                {
                    data->percentage = (uint8_t)(config->entries[i - 1].percentage - config->entries[i].percentage) / (config->entries[i - 1].voltage - config->entries[i].voltage) * (oneCellVoltage - config->entries[i].voltage) + config->entries[i].percentage;

                    return;
                }
            }

            data->percentage = oneCellVoltage >= config->entries[0].voltage ? 100 : 0;
        }
        else
        {
            data->percentage = 0;
        }
    }
}