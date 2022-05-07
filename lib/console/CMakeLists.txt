add_library(console
    console_input.h
    console_input.c
    console_log_printer.h
    console_log_printer.c
    )

target_link_libraries(console pico_stdlib logging console_colors)

target_include_directories(console PUBLIC .)