set(LIB_GENERAL_NAME kernel_tests_lib)

add_library(${LIB_GENERAL_NAME} main.c)

target_link_libraries(${LIB_GENERAL_NAME} INTERFACE
    tests_kernel_commands
    tests_kernel_logging
    tests_kernel_services
)