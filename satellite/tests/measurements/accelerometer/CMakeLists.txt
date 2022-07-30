add_library(accelerometer_tests test_accelerometer.h test_accelerometer.c)

target_link_libraries(accelerometer_tests PUBLIC measurements time_tracker test_framework)

target_include_directories(accelerometer_tests PUBLIC .)