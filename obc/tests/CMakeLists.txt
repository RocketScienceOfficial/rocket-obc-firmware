link_libraries(api_lib drivers_console_lib)

add_subdirectory(test_framework)

link_libraries(test_framework)

add_subdirectory(crypto_tests)
add_subdirectory(drivers_tests)
add_subdirectory(kernel_tests)