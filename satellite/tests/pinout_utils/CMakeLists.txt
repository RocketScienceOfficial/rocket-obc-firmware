add_library(pinout_utils_tests test_pinout_utils.h test_pinout_utils.c)

target_link_libraries(pinout_utils_tests PUBLIC pinout_utils_lib test_framework)

target_include_directories(pinout_utils_tests PUBLIC .)