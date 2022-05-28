add_executable(servoTest main.c)

target_link_libraries(servoTest PUBLIC pico_stdlib logging servo myassert console_colors)

pico_enable_stdio_usb(servoTest 1)
pico_enable_stdio_uart(servoTest 0)

pico_add_extra_outputs(servoTest)