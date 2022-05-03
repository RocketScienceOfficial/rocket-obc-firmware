add_library(misc
    time_tracker.h
    time_tracker.c
    console_commands.h
    console_commands.c
    console_input.h
    console_input.c
    my_event.h
    my_event.c
    )

target_link_libraries(misc pico_stdlib hardware_clocks sd)
target_include_directories(misc PUBLIC .)