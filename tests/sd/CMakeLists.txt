add_executable(sdTest main.c hw_config.c)

target_link_libraries(sdTest PUBLIC pico_stdlib logging sd myassert)

pico_enable_stdio_usb(sdTest 1)
pico_enable_stdio_uart(sdTest 0)

pico_add_extra_outputs(sdTest)