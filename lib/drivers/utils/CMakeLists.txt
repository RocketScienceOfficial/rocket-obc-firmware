obc_add_lib(drivers_utils gpio_utils.c)

target_link_libraries(${LIB_NAME} INTERFACE obc_hal)