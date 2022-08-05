add_library(barometer_tests test_barometer.h test_barometer.c)

target_link_libraries(barometer_tests PUBLIC barometer_lib time_tracker_lib test_framework)

target_include_directories(barometer_tests PUBLIC .)