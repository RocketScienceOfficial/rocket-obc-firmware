obc_add_submodule(utils drivers gpio_utils.c)

target_link_libraries(${MODULE_NAME} INTERFACE module_drivers_hal)