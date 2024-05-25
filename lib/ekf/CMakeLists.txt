obc_add_lib(ekf ekf.c)

target_link_libraries(${LIB_NAME} INTERFACE lib_maths lib_geo)