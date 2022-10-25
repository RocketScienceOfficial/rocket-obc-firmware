set(LIB_GENERAL_NAME drivers_tests_lib)

add_library(${LIB_GENERAL_NAME} main.c)

target_link_libraries(${LIB_GENERAL_NAME} INTERFACE
    tests_drivers_console
    tests_drivers_storage
)