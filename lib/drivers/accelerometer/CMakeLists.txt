obc_add_lib(drivers_accel h3lis331dl_driver.c)

target_link_libraries(${LIB_NAME} INTERFACE obc_hal lib_drivers_utils lib_maths lib_geo)