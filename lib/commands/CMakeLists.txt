add_library(commands
    console_commands.h
    console_commands.c
    default_console_commands.h
    default_console_commands.c
    commands_printer.h
    commands_printer.c
    )

target_link_libraries(commands logging console_colors)

target_include_directories(commands PUBLIC .)