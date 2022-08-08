add_library(accelerometer_tests STATIC
    test_accelerometer.h
    test_accelerometer.c
    )

target_link_libraries(accelerometer_tests PRIVATE hardware_lib utils_lib)
target_include_directories(accelerometer_tests PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})