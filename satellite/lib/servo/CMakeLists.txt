add_library(servo
    mg995.c
    mg995.h
    )

target_link_libraries(servo pico_stdlib hardware_pwm)
target_include_directories(servo PUBLIC .)