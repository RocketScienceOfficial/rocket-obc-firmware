add_library(kernel_commands_lib STATIC
    include/kernel/commands/commands.h
    include/kernel/commands/default_commands.h
    src/commands.c
    src/default_commands.c
    )

target_link_libraries(kernel_commands_lib PRIVATE pico_stdlib utils_lib)
target_include_directories(kernel_commands_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)