set(EXE_NAME satellite)

add_executable(${EXE_NAME}
    main.c
    hw_config.c
    measurements_manager.c
    mission_control.c
)

target_link_libraries(${EXE_NAME} PRIVATE core_shared_lib)

pico_enable_stdio_usb(${EXE_NAME} 1)
pico_enable_stdio_uart(${EXE_NAME} 0)
pico_add_extra_outputs(${EXE_NAME})