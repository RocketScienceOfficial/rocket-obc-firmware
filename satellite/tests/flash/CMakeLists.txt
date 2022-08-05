add_library(flash_tests test_flash.h test_flash.c)

target_link_libraries(flash_tests PUBLIC flash_lib test_framework)

target_include_directories(flash_tests PUBLIC .)