add_executable(mpu6050ConvertedTest main.c)

target_link_libraries(mpu6050ConvertedTest PUBLIC pico_stdlib time_tracker logging measurements myassert)

pico_enable_stdio_usb(mpu6050ConvertedTest 1)
pico_enable_stdio_uart(mpu6050ConvertedTest 0)

pico_add_extra_outputs(mpu6050ConvertedTest)