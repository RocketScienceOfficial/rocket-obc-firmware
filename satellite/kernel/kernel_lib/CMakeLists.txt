set(LIB_GENERAL_NAME kernel_lib)

add_library(${LIB_GENERAL_NAME} main.c)

target_link_libraries(${LIB_GENERAL_NAME} INTERFACE
    kernel_logging_lib
    kernel_commands_lib
    kernel_services_lib
)