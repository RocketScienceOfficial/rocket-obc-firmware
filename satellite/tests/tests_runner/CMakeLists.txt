set(EXE_NAME tests_runner)

add_executable(${EXE_NAME}
    main.c
)

target_link_libraries(${EXE_NAME} PRIVATE pico_stdlib tools_lib drivers_lib kernel_lib test_framework drivers_tests_lib kernel_tests_lib tests_tools)

pico_enable_stdio_usb(${EXE_NAME} 1)
pico_enable_stdio_uart(${EXE_NAME} 0)
pico_add_extra_outputs(${EXE_NAME})