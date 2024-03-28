obc_add_module(geo geo_mag.c geo.c geo_utils.c)

target_link_libraries(${MODULE_NAME} INTERFACE module_maths)