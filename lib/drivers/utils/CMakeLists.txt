obc_add_sublib(utils drivers gpio_utils.c)

target_link_libraries(${LIB_NAME} INTERFACE obc_hal)