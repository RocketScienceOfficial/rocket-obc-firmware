obc_add_submodule(hal drivers adc_driver.c board_control.c flash_hal_driver.c gpio_driver.c gpio_driver.c gpio_utils.c i2c_driver.c multithreading.c pwm_driver.c serial_driver.c spi_driver.c time_tracker.c)

target_link_libraries(${MODULE_NAME} INTERFACE hardware_adc hardware_i2c hardware_pwm hardware_spi hardware_flash hardware_sync pico_multicore pico_stdlib)