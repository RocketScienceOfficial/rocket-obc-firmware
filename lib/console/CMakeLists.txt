add_library(console
    console_input.h
    console_input.c
    console_printer.h
    console_printer.c
    )

target_link_libraries(console pico_stdlib logging console_colors)

target_include_directories(console PUBLIC .)