add_library(time_tracker
    time_tracker.c
    time_tracker.h
    )

target_link_libraries(time_tracker pico_stdlib)
target_include_directories(time_tracker PUBLIC .)