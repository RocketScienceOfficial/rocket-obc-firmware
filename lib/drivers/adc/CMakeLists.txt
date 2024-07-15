obc_add_lib(drivers_adc ads7038_driver.c)

target_link_libraries(${LIB_NAME} INTERFACE obc_hal)