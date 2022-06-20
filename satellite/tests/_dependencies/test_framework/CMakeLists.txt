add_library(test_framework INTERFACE
    test_framework.h
    )

target_link_libraries(test_framework INTERFACE logging)

target_include_directories(test_framework INTERFACE .)