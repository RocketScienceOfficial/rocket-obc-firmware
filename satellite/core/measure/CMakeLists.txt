add_executable(measure
    main.c
    hw_config.c
    measurements_manager.c
    radio_controller.c
)

target_link_libraries(measure PUBLIC core_shared pico_stdlib time_tracker_lib logging_lib maths_lib accelerometer_lib barometer_lib gps_lib radio_lib sd_lib console_lib commands_lib flash_lib)

pico_enable_stdio_usb(measure 1)
pico_enable_stdio_uart(measure 0)

pico_add_extra_outputs(measure)