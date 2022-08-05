add_library(commands_tests test_commands.h test_commands.c)

target_link_libraries(commands_tests PUBLIC commands_lib test_framework)

target_include_directories(commands_tests PUBLIC .)