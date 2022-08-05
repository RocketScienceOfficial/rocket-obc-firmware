add_library(barometer_lib
    barometer.c
    barometer.h
)

target_link_libraries(barometer_lib errors_lib maths_lib pinout_utils_lib pico_stdlib hardware_i2c)
target_include_directories(barometer_lib PUBLIC .)