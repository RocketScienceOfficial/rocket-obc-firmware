add_library(commands
    console_commands.h
    console_commands.c
    default_console_commands.h
    default_console_commands.c
    )

target_link_libraries(commands logging)

target_include_directories(commands PUBLIC .)