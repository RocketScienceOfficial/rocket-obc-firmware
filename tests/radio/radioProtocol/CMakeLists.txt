add_executable(radioProtocolTest main.c)

target_link_libraries(radioProtocolTest PUBLIC pico_stdlib logging radio test_framework)

pico_enable_stdio_usb(radioProtocolTest 1)
pico_enable_stdio_uart(radioProtocolTest 0)

pico_add_extra_outputs(radioProtocolTest)