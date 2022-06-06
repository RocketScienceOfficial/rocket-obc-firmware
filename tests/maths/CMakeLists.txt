add_executable(mathsTest main.c)

target_link_libraries(mathsTest PUBLIC pico_stdlib logging maths myassert)

pico_enable_stdio_usb(mathsTest 1)
pico_enable_stdio_uart(mathsTest 0)

pico_add_extra_outputs(mathsTest)