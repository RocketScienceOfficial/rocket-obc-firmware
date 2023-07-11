link_libraries(api_lib)

add_subdirectory(test_framework)

link_libraries(test_framework)

add_subdirectory(drivers_tests)
add_subdirectory(services_tests)

add_subdirectory(tests_runner)