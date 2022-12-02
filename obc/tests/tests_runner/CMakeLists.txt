set(EXE_NAME tests_runner)

add_executable(${EXE_NAME}
    main.c
)

target_link_libraries(${EXE_NAME} PRIVATE pico_stdlib drivers_lib kernel_lib test_framework tests_crypto drivers_tests_lib kernel_tests_lib)

pico_enable_stdio_usb(${EXE_NAME} 1)
pico_enable_stdio_uart(${EXE_NAME} 0)
pico_add_extra_outputs(${EXE_NAME})