obc_add_module(ekf ekf.c)

target_link_libraries(${MODULE_NAME} INTERFACE module_maths module_geo)