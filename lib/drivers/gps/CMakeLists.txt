obc_add_lib(drivers_gps gps_driver.c nmea.c)

target_link_libraries(${LIB_NAME} INTERFACE obc_hal lib_drivers_utils lib_geo)