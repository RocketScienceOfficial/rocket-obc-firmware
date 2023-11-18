obc_add_submodule(servo drivers pwm_servo_driver.c)

target_link_libraries(${MODULE_NAME} INTERFACE module_drivers_hal)