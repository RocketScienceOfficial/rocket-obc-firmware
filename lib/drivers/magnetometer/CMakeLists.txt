obc_add_lib(drivers_magnetometer mmc5983ma_driver.c)

target_link_libraries(${LIB_NAME} INTERFACE obc_hal lib_drivers_utils lib_maths)