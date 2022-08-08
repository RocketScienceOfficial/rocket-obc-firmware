add_library(console_tests STATIC
    test_console.h
    test_console.c
    )

target_link_libraries(console_tests PRIVATE logging_lib console_lib)
target_include_directories(console_tests PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})