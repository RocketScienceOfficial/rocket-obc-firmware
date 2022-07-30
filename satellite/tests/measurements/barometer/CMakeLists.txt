add_library(barometer_tests test_barometer.h test_barometer.c)

target_link_libraries(barometer_tests PUBLIC measurements time_tracker test_framework)

target_include_directories(barometer_tests PUBLIC .)