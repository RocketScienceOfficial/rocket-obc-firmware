add_library(servo_tests STATIC
    test_servo.h
    test_servo.c
    )

target_link_libraries(servo_tests PRIVATE hardware_lib)
target_include_directories(servo_tests PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})