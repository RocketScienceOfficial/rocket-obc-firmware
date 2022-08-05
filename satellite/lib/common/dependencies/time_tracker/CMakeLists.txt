add_library(time_tracker_lib
    time_tracker.c
    time_tracker.h
    )

target_link_libraries(time_tracker_lib pico_stdlib)
target_include_directories(time_tracker_lib PUBLIC .)