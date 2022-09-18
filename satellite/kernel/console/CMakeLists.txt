add_library(kernel_console_lib STATIC
    include/kernel/console/console_input.h
    include/kernel/console/console_output.h
    src/console_input.c
    src/console_output.c
    )

target_link_libraries(kernel_console_lib PUBLIC pico_stdlib utils_lib)
target_include_directories(kernel_console_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)