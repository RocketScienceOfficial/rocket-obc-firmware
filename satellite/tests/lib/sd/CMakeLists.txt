add_library(sd_tests STATIC
    test_sd.h
    test_sd.c
    hw_config.c
    )

target_link_libraries(sd_tests PRIVATE hardware_lib utils_lib logging_lib)
target_include_directories(sd_tests PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})