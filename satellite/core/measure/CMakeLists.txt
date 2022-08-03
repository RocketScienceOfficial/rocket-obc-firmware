add_executable(measure
    main.c
    hw_config.c
    sd_manager.c
    measurements_manager.c
    radio_controller.c
    measurement_logger.c
)

target_link_libraries(measure PUBLIC core_shared pico_stdlib time_tracker logging maths measurements radio sd console commands flash)

pico_enable_stdio_usb(measure 1)
pico_enable_stdio_uart(measure 0)

pico_add_extra_outputs(measure)