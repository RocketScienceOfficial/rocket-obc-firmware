add_library(sd_tests test_sd.h test_sd.c hw_config.c)

target_link_libraries(sd_tests PUBLIC logging sd test_framework)

target_include_directories(sd_tests PUBLIC .)