obc_add_sublib(storage drivers flash_driver.c)

target_link_libraries(${LIB_NAME} INTERFACE obc_hal)