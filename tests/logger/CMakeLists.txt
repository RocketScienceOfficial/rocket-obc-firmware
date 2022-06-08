add_executable(loggerTest main.c)

target_link_libraries(loggerTest PUBLIC pico_stdlib logging test_framework)

pico_enable_stdio_usb(loggerTest 1)
pico_enable_stdio_uart(loggerTest 0)

pico_add_extra_outputs(loggerTest)