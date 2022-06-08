add_executable(radioSenderTest main.c)

target_link_libraries(radioSenderTest PUBLIC pico_stdlib time_tracker logging radio test_framework)

pico_enable_stdio_usb(radioSenderTest 1)
pico_enable_stdio_uart(radioSenderTest 0)

pico_add_extra_outputs(radioSenderTest)