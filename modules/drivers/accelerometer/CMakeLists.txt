obc_add_submodule(accel drivers bmi088_driver.c h3lis331dl_driver.c lsm6dso32_driver.c)

target_link_libraries(${MODULE_NAME} INTERFACE module_drivers_hal module_drivers_utils module_maths)