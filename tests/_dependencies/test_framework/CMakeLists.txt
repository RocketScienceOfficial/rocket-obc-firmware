add_library(test_framework 
    test_framework.h
    test_framework.c
    )

target_link_libraries(test_framework logging)

target_include_directories(test_framework PUBLIC .)