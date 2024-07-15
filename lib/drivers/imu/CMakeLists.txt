obc_add_lib(drivers_imu bmi088_driver.c lsm6dso32_driver.c)

target_link_libraries(${LIB_NAME} INTERFACE obc_hal lib_drivers_utils lib_maths lib_geo)