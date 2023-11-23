obc_add_submodule(magnetometer drivers mmc5983ma_driver.c)

target_link_libraries(${MODULE_NAME} INTERFACE module_drivers_hal module_drivers_utils module_maths)