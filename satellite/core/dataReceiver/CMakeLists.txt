add_executable(dataReceiver
    main.c
    radio_controller.c
    receiver_logger.c
)

target_link_libraries(dataReceiver PUBLIC core_shared pico_stdlib time_tracker_lib error_handling_lib logging_lib maths_lib accelerometer_lib barometer_lib gps_lib radio_lib sd_lib console_lib commands_lib flash_lib servo_lib)

pico_enable_stdio_usb(dataReceiver 1)
pico_enable_stdio_uart(dataReceiver 0)

pico_add_extra_outputs(dataReceiver)