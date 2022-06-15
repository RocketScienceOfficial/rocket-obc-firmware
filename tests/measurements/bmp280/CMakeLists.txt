add_library(bmp280_tests test_bmp280.h test_bmp280.c)

target_link_libraries(bmp280_tests PUBLIC measurements time_tracker test_framework)

target_include_directories(bmp280_tests PUBLIC .)