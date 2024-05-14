obc_add_sublib(accel drivers bmi088_driver.c h3lis331dl_driver.c lsm6dso32_driver.c)

target_link_libraries(${LIB_NAME} INTERFACE obc_hal lib_drivers_utils lib_maths lib_geo)