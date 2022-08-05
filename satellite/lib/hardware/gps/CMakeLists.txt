add_library(gps_lib
    gps.c
    gps.h
)

target_link_libraries(gps_lib errors_lib pico_stdlib hardware_i2c)
target_include_directories(gps_lib PUBLIC .)