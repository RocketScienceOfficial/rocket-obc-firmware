obc_add_lib(drivers_servo pwm_servo_driver.c)

target_link_libraries(${LIB_NAME} INTERFACE obc_hal)