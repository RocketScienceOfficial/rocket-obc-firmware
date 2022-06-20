add_library(console_tests test_console.h test_console.c)

target_link_libraries(console_tests PUBLIC logging console test_framework)

target_include_directories(console_tests PUBLIC .)