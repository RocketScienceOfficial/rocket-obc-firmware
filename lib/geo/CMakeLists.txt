obc_add_lib(geo geo_mag.c geo.c geo_utils.c)

target_link_libraries(${LIB_NAME} INTERFACE lib_maths)