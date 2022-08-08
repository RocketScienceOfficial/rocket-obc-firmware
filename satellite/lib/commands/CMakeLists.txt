add_library(commands_lib STATIC
    include/commands/commands.h
    include/commands/default_commands.h
    src/commands.c
    src/default_commands.c
    )

target_link_libraries(commands_lib PRIVATE logging_lib)
target_include_directories(commands_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)