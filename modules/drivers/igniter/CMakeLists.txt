obc_add_submodule(igniter drivers igniter_driver.c)

target_link_libraries(${MODULE_NAME} INTERFACE module_drivers_hal)