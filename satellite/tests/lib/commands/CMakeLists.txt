add_library(commands_tests STATIC
    test_commands.h
    test_commands.c
    )

target_link_libraries(commands_tests PRIVATE commands_lib)
target_include_directories(commands_tests PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})