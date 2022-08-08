add_executable(static_tests_runner main.c)

pico_enable_stdio_usb(static_tests_runner 1)
pico_enable_stdio_uart(static_tests_runner 0)
pico_add_extra_outputs(static_tests_runner)