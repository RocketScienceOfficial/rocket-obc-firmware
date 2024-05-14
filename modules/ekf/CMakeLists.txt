obc_add_module(ekf ekf.c)

target_link_libraries(${MODULE_NAME} INTERFACE lib_maths lib_geo)