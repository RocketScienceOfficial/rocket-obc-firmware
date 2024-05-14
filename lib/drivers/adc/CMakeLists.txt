obc_add_sublib(adc drivers ads7038_q1_driver.c)

target_link_libraries(${LIB_NAME} INTERFACE obc_hal)