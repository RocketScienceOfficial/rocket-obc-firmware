add_library(console
    console_input.h
    console_input.c
    )

target_link_libraries(console pico_stdlib logging)

target_include_directories(console PUBLIC .)