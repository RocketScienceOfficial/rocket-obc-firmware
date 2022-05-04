add_executable(measure main.c hw_config.c)

target_link_libraries(measure PUBLIC pico_stdlib logging maths measurements radio sd console myassert)

pico_enable_stdio_usb(measure 1)
pico_enable_stdio_uart(measure 0)

pico_add_extra_outputs(measure)