obc_add_sublib(magnetometer drivers mmc5983ma_driver.c)

target_link_libraries(${LIB_NAME} INTERFACE obc_hal lib_drivers_utils lib_maths)