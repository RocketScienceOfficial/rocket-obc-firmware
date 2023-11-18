obc_add_submodule(adc drivers max1161X_driver.c)

target_link_libraries(${MODULE_NAME} INTERFACE module_drivers_hal)