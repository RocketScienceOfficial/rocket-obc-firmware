add_library(kernel_lib main.c)

target_link_libraries(kernel_lib INTERFACE
    kernel_logging_lib
    kernel_commands_lib
    kernel_console_lib
    kernel_services_lib
)