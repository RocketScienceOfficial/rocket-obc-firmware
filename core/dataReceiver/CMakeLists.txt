add_executable(dataReceiver main.c)

target_link_libraries(dataReceiver PUBLIC pico_stdlib logging maths radio servo myassert console commands console_colors)

pico_enable_stdio_usb(dataReceiver 1)
pico_enable_stdio_uart(dataReceiver 0)

pico_add_extra_outputs(dataReceiver)