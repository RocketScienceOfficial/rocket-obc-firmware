add_library(servo_tests test_servo.h test_servo.c)

target_link_libraries(servo_tests PUBLIC servo_lib test_framework)

target_include_directories(servo_tests PUBLIC .)