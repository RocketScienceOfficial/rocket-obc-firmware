obc_add_submodule(storage drivers flash_driver.c sd_driver.c)

target_link_libraries(${MODULE_NAME} INTERFACE module_drivers_hal FatFs_SPI)