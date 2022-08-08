add_executable(measure
    main.c
    hw_config.c
    measurements_manager.c
    radio_controller.c
)

target_link_libraries(measure PRIVATE core_shared)

pico_enable_stdio_usb(measure 1)
pico_enable_stdio_uart(measure 0)
pico_add_extra_outputs(measure)