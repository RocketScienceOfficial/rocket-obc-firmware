obc_add_module(nmea nmea_parser.c nmea_wrapper.c)

target_link_libraries(${MODULE_NAME} INTERFACE module_drivers_gps module_logger)