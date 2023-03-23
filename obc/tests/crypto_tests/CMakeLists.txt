set(EXE_NAME crypto_tests)

add_executable(${EXE_NAME}
    main.c
    test_cryptography.c
)

target_link_libraries(${EXE_NAME} PRIVATE crypto_lib)

pico_enable_stdio_usb(${EXE_NAME} 1)
pico_enable_stdio_uart(${EXE_NAME} 0)
pico_add_extra_outputs(${EXE_NAME})