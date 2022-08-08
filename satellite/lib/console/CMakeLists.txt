add_library(console_lib STATIC
    include/console/console_input.h
    src/console_input.c
    )

target_link_libraries(console_lib PRIVATE pico_stdlib logging_lib)
target_include_directories(console_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)