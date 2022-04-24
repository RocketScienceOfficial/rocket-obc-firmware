add_library(misc
    config.h
    config.c
    time_tracker.h
    time_tracker.c
    )

target_link_libraries(misc pico_stdlib hardware_clocks sd)
target_include_directories(misc PUBLIC .)