add_executable(radioReceiverTest main.c)

target_link_libraries(radioReceiverTest PUBLIC pico_stdlib logging radio myassert)

pico_enable_stdio_usb(radioReceiverTest 1)
pico_enable_stdio_uart(radioReceiverTest 0)

pico_add_extra_outputs(radioReceiverTest)