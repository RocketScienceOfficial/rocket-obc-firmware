obc_add_lib(nmea nmea_parser.c)

target_link_libraries(${LIB_NAME} INTERFACE lib_drivers_gps)