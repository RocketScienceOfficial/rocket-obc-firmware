add_library(servo_lib
    servo.c
    servo.h
    )

target_link_libraries(servo_lib errors_lib pinout_utils_lib pico_stdlib hardware_pwm)
target_include_directories(servo_lib PUBLIC .)