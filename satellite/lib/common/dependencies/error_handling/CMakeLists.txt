add_library(error_handling_lib INTERFACE
    error_handling.h
    )

target_link_libraries(error_handling_lib INTERFACE pico_stdlib)
target_include_directories(error_handling_lib INTERFACE .)