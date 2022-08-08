add_library(maths_tests STATIC
    test_maths.h
    test_maths.c
    )

target_link_libraries(maths_tests PRIVATE utils_lib)
target_include_directories(maths_tests PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})