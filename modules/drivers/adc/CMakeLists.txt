obc_add_submodule(adc drivers ads7038_q1_driver.c)

target_link_libraries(${MODULE_NAME} INTERFACE module_drivers_hal module_logger)