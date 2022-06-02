add_executable(mpu6050FilteredTest main.c)

target_link_libraries(mpu6050FilteredTest PUBLIC pico_stdlib time_tracker logging maths measurements myassert)

pico_enable_stdio_usb(mpu6050FilteredTest 1)
pico_enable_stdio_uart(mpu6050FilteredTest 0)

pico_add_extra_outputs(mpu6050FilteredTest)