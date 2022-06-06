add_executable(consoleTest main.c)

target_link_libraries(consoleTest PUBLIC pico_stdlib console logging myassert)

pico_enable_stdio_usb(consoleTest 1)
pico_enable_stdio_uart(consoleTest 0)

pico_add_extra_outputs(consoleTest)