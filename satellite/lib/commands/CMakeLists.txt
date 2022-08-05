add_library(commands_lib
    commands.h
    commands.c
    default_commands.h
    default_commands.c
    )

target_link_libraries(commands_lib logging_lib)

target_include_directories(commands_lib PUBLIC .)