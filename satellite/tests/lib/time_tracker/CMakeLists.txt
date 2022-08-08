add_library(time_tracker_tests STATIC
    test_time_tracker.h
    test_time_tracker.c
    )

target_link_libraries(time_tracker_tests PRIVATE utils_lib)
target_include_directories(time_tracker_tests PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})