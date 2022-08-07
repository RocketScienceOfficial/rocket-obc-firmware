add_library(logger_tests test_logger.h test_logger.c)

target_link_libraries(logger_tests PUBLIC logging_lib test_framework)

target_include_directories(logger_tests PUBLIC .)