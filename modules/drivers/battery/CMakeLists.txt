obc_add_submodule(battery drivers battery_driver.c)

target_link_libraries(${MODULE_NAME} INTERFACE module_drivers_hal)