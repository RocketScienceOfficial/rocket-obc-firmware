obc_add_submodule(barometer drivers bme688_driver.c bmp581_driver.c)

target_link_libraries(${MODULE_NAME} INTERFACE module_drivers_hal module_drivers_utils)