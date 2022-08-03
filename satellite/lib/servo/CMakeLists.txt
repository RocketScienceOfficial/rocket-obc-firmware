add_library(servo
    servo.c
    servo.h
    )

target_link_libraries(servo pico_stdlib hardware_pwm logging)
target_include_directories(servo PUBLIC .)