obc_add_sublib(battery drivers battery_driver.c)

target_link_libraries(${LIB_NAME} INTERFACE obc_hal)