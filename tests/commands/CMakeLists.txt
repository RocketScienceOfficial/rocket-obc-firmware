add_executable(commandsTest main.c)

target_link_libraries(commandsTest PUBLIC pico_stdlib commands logging test_framework)

pico_enable_stdio_usb(commandsTest 1)
pico_enable_stdio_uart(commandsTest 0)

pico_add_extra_outputs(commandsTest)