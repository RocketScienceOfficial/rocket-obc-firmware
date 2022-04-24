add_library(measurements
    bmp280.c
    bmp280.h
    mpu6050.c
    mpu6050.h
    )

target_link_libraries(measurements misc logging maths pico_stdlib hardware_i2c)
target_include_directories(measurements PUBLIC .)