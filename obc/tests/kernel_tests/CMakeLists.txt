set(EXE_NAME kernel_tests)

add_executable(${EXE_NAME}
    main.c
    test_commands.c
    test_logger.c
    test_radio_protocol.c
)

target_link_libraries(${EXE_NAME} PRIVATE kernel_lib pico_stdlib)

pico_enable_stdio_usb(${EXE_NAME} 1)
pico_enable_stdio_uart(${EXE_NAME} 0)
pico_add_extra_outputs(${EXE_NAME})