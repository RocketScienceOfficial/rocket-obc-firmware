obc_add_lib(drivers_buzzer active_buzzer_driver.c passive_buzzer_driver.c)

target_link_libraries(${LIB_NAME} INTERFACE obc_hal)