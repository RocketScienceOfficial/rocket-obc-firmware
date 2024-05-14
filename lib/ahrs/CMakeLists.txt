obc_add_lib(ahrs madgwick_filter.c mahony_filter.c)

target_link_libraries(${LIB_NAME} INTERFACE lib_maths)