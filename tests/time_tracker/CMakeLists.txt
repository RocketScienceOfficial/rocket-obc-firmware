add_library(time_tracker_test test_time_tracker.h test_time_tracker.c)

target_link_libraries(time_tracker_test PUBLIC time_tracker test_framework)

target_include_directories(time_tracker_test PUBLIC .)