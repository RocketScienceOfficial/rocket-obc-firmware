add_executable(mpu6050RawTest main.c)

target_link_libraries(mpu6050RawTest PUBLIC pico_stdlib time_tracker logging measurements myassert)

pico_enable_stdio_usb(mpu6050RawTest 1)
pico_enable_stdio_uart(mpu6050RawTest 0)

pico_add_extra_outputs(mpu6050RawTest)