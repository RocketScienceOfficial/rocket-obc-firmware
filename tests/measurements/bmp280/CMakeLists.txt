add_executable(bmp280Test main.c)

target_link_libraries(bmp280Test PUBLIC pico_stdlib time_tracker logging measurements myassert)

pico_enable_stdio_usb(bmp280Test 1)
pico_enable_stdio_uart(bmp280Test 0)

pico_add_extra_outputs(bmp280Test)