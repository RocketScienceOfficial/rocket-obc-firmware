add_library(barometer_tests STATIC
    test_barometer.h
    test_barometer.c
    )

target_link_libraries(barometer_tests PRIVATE hardware_lib utils_lib)
target_include_directories(barometer_tests PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})