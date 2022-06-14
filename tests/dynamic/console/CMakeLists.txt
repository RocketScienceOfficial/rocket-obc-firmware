add_executable(console_dynamic_tests main.c)

target_include_directories(dataReceiver PUBLIC ../)

target_link_libraries(dataReceiver PUBLIC pico_stdlib time_tracker logging maths radio servo console commands measurements)

pico_enable_stdio_usb(dataReceiver 1)
pico_enable_stdio_uart(dataReceiver 0)

pico_add_extra_outputs(dataReceiver)