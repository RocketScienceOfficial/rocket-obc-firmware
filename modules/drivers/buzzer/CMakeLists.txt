obc_add_submodule(buzzer drivers active_buzzer_driver.c passive_buzzer_driver.c)

target_link_libraries(${MODULE_NAME} INTERFACE module_drivers_hal)