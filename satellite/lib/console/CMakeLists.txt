add_library(console_lib
    console_input.h
    console_input.c
    )

target_link_libraries(console_lib pico_stdlib logging_lib)

target_include_directories(console_lib PUBLIC .)