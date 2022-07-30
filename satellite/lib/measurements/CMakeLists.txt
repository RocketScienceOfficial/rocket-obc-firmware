add_library(measurements
    barometer.c
    barometer.h
    accelerometer.c
    accelerometer.h
    gps.c
    gps.h
)

target_link_libraries(measurements logging maths pico_stdlib hardware_i2c)
target_include_directories(measurements PUBLIC .)