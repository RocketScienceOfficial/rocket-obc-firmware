add_executable(dynamic_tests_runner main.c)

pico_enable_stdio_usb(dynamic_tests_runner 1)
pico_enable_stdio_uart(dynamic_tests_runner 0)
pico_add_extra_outputs(dynamic_tests_runner)