add_library(time_tracker_tests test_time_tracker.h test_time_tracker.c)

target_link_libraries(time_tracker_tests PUBLIC time_tracker_lib test_framework)

target_include_directories(time_tracker_tests PUBLIC .)