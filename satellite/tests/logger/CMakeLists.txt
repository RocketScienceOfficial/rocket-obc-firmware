add_library(loggers_tests test_logger.h test_logger.c)

target_link_libraries(loggers_tests PUBLIC logging_lib test_framework)

target_include_directories(loggers_tests PUBLIC .)