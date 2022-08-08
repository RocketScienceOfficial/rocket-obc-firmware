add_library(logger_tests STATIC
    test_logger.h
    test_logger.c
    )

target_link_libraries(logger_tests PRIVATE logging_lib)
target_include_directories(logger_tests PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})