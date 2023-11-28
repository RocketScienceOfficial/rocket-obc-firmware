obc_add_submodule(hal drivers
    ${OBC_PLATFORM}/adc_driver.c
    ${OBC_PLATFORM}/board_control.c
    ${OBC_PLATFORM}/flash_hal_driver.c
    ${OBC_PLATFORM}/gpio_driver.c
    ${OBC_PLATFORM}/i2c_driver.c
    ${OBC_PLATFORM}/multicore.c
    ${OBC_PLATFORM}/pwm_driver.c
    ${OBC_PLATFORM}/serial_driver.c
    ${OBC_PLATFORM}/spi_driver.c
    ${OBC_PLATFORM}/time_tracker.c
)

target_link_libraries(${MODULE_NAME} INTERFACE hardware_adc hardware_i2c hardware_pwm hardware_spi hardware_flash hardware_sync pico_multicore pico_stdlib)