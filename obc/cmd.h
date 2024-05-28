#ifndef _CMD_H
#define _CMD_H

/**
 * @brief Command type
 */
typedef enum cmd_type
{
    CMD_UNKNOWN = -1,
    CMD_DATA_READ = 0,
} cmd_type_t;

/**
 * @brief Initialize command system
 */
void cmd_init(void);

/**
 * @brief Update command system, gather data
 */
void cmd_update(void);

/**
 * @brief Get current command type
 * 
 * @return Type of command
 */
cmd_type_t get_current_cmd(void);

#endif