obc_add_sublib(servo drivers pwm_servo_driver.c)

target_link_libraries(${LIB_NAME} INTERFACE obc_hal)