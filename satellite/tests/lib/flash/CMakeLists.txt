add_library(flash_tests STATIC
    test_flash.h
    test_flash.c
    )

target_link_libraries(flash_tests PRIVATE hardware_lib)
target_include_directories(flash_tests PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})