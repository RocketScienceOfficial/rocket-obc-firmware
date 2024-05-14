obc_add_sublib(lora drivers sx126X_driver.c sx127X_driver.c)

target_link_libraries(${LIB_NAME} INTERFACE obc_hal)