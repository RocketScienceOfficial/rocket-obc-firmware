obc_add_lib(battery battery_utils.c)

target_link_libraries(${LIB_NAME} INTERFACE obc_hal)