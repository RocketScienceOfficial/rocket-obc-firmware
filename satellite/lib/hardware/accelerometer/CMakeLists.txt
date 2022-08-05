add_library(accelerometer_lib
    accelerometer.c
    accelerometer.h
)

target_link_libraries(accelerometer_lib errors_lib maths_lib pinout_utils_lib pico_stdlib hardware_i2c)
target_include_directories(accelerometer_lib PUBLIC .)