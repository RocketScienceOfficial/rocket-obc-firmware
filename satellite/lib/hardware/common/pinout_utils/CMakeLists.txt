add_library(pinout_utils_lib
    pinout_utils.h
    pinout_utils.c
    )

target_link_libraries(pinout_utils_lib pico_stdlib)
target_include_directories(pinout_utils_lib PUBLIC .)