add_library(pinout_utils_tests STATIC
    test_pinout_utils.h
    test_pinout_utils.c
    )

target_link_libraries(pinout_utils_tests PRIVATE utils_lib)
target_include_directories(pinout_utils_tests PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})