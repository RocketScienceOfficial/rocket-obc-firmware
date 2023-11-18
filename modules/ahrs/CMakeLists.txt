obc_add_module(ahrs madgwick_filter.c mahony_filter.c)

target_link_libraries(${MODULE_NAME} INTERFACE module_maths)